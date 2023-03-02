import json, sys, os, re

dir_path = os.path.dirname(os.path.realpath(__file__))

sourcePath = os.path.join(dir_path, "../NodeJS/Server/build/generated", "swagger.json")
targetPath = os.path.join(dir_path, "../Source/DTHttpClient/Public/Generated")

if not os.path.exists(sourcePath):
    sys.exit(f'Source path /"{sourcePath}/" not found')

sourceFile = open(sourcePath)
sourceJson = json.load(sourceFile)

types = {}

def sanitizeName(name):
    if len(name) == 0: return ""
    return name[0].upper() + name[1:]

def sanitizeDesc(desc):
    return desc.replace('/n', ' ').replace('\r', '')

# Check whether property is an array
def propertyIsArray(property):
    
    type = property['type'] if 'type' in property else "string"
    return type == "array"

# Convert property object to type name
def propertyToTypeName(property):

    if '$ref' in property:
        ref = property['$ref']
        return types[ref] if ref in types else 'FString'
    
    type = property['type'] if 'type' in property else "string"
    format = property['format'] if 'format' in property else ""

    if type == "string":
        if format == "date-time":
            return 'FDateTime'
        if format == "byte":
            return 'TArray<uint8>'
        if format == "binary":
            return 'TArray<uint8>'
        return 'FString'

    elif type == "number":
        # We don't differentiate between float/double format
        return 'float'

    elif type == "integer":
        if format == "int32":
            return 'int32'
        if format == "int64":
            return 'int64'
        return 'int32'

    elif type == "boolean":
        return 'bool'
    
    elif type == "array":
        subType = propertyToTypeName(property['items'])
        return f'TArray<{subType}>'
    
    return ''

# Print models
modelData = """
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"

#include "DTModels.generated.h"
"""

for schemaName, schema in sourceJson['components']['schemas'].items():

    schemaDesc = sanitizeDesc(schema['description'] if 'description' in schema else "")
    schemaPath = f'#/components/schemas/{schemaName}'

    if 'enum' in schema:
        types[schemaPath] = f'EDT{schemaName}'
        
        modelData += f'''

/** {schemaDesc} */
UENUM()
enum class EDT{schemaName} : uint8
{{'''
        switchData = f''
        unswitchData = f''

        if schema['type'] == 'string':
            for entry in schema['enum']:
                modelData += f'''
    {sanitizeName(entry)},'''
                switchData += f'''
    case EDT{schemaName}::{sanitizeName(entry)}: return TEXT("{entry}");'''
                unswitchData += f'''
    if (Name.Equals(TEXT("{entry}"))) Value = EDT{schemaName}::{sanitizeName(entry)}; else'''

        elif schema['type'] == 'number': 
            modelData += f'''
    None,'''
            for entry in schema['enum']:
                modelData += f'''
    Code{entry},'''
                switchData += f'''
    case EDT{schemaName}::Code{entry}: return TEXT("{entry}");'''
                unswitchData += f'''
    if (Name.Equals(TEXT("{entry}"))) Value = EDT{schemaName}::Code{entry}; else'''

        modelData += f'''
}};

FORCEINLINE FString LexToString(EDT{schemaName} Value)
{{
    switch (Value)
    {{{switchData}
    default: return TEXT("");
    }}
}};

FORCEINLINE void LexFromString(EDT{schemaName}& Value, const TCHAR* Buffer)
{{
    FString Name = FString(Buffer);{unswitchData} {{}}
}};'''

    elif schema['type'] == 'object':
        types[schemaPath] = f'FDT{schemaName}'

        modelData += f'''

/** {schemaDesc} */
USTRUCT()
struct DTHTTPCLIENT_API FDT{schemaName}
{{
    GENERATED_USTRUCT_BODY()'''
        
        for propertyName, property in schema['properties'].items():
            
            propertyDesc = sanitizeDesc(property['description'] if 'description' in property else "")
            
            modelData += f'''

    /** {propertyDesc} */
    UPROPERTY()
        {propertyToTypeName(property)} {sanitizeName(propertyName)};'''

        modelData += f'''
}};

FORCEINLINE FString LexToString(const FDT{schemaName}& Value)
{{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDT{schemaName}>(Value, Output);
    return *Output;
}};

FORCEINLINE void LexFromString(FDT{schemaName}& Value, const TCHAR* Buffer)
{{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDT{schemaName}>(Buffer, &Value);
}};

FORCEINLINE void LexFromString(TArray<FDT{schemaName}>& Value, const TCHAR* Buffer)
{{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDT{schemaName}>(Buffer, &Value);
}};'''
    
    elif not schemaPath in types:
        typeName = propertyToTypeName(schema)
        types[schemaPath] = typeName

modelTargetPath = os.path.join(targetPath, "DTModels.h")
f = open(modelTargetPath, "w")
f.write(modelData)
f.close()

print(f'Generated Models to {modelTargetPath}')

# Print api
apiData = f'''
#pragma once

#include "Http/DTHttpRequester.h"
#include "Http/DTHttpContext.h"
#include "DTModels.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTApi.generated.h"

/**
 * 
 */
UCLASS()
class DTHTTPCLIENT_API UDTApi : public UBlueprintFunctionLibrary
{{
	GENERATED_BODY()
public:'''

for pathName, path in sourceJson['paths'].items():

    for operationName, operation in path.items():

        sanitizedPath = "".join([sanitizeName(s) for s in pathName.split('/')])
        functionName = re.sub(r'{.*}', "With", sanitizeName(operationName) + sanitizedPath)
        
        operationDesc = sanitizeDesc(operation['description'] if 'description' in operation else "")

        bodyType = (propertyToTypeName(operation['requestBody']['content']['application/json']['schema'])
            if  'requestBody' in operation else 'FEmptyBody')

        returnType = (propertyToTypeName(operation['responses']['200']['content']['application/json']['schema'])
            if  '200' in operation['responses'] and 
                'content' in operation['responses']['200'] else 'FEmptyResult')
        
        functionParams = ""
        functionParamArgs = ""
        functionParamPath = ""
        for parameter in operation['parameters']:

            if parameter['in'] == 'path':

                parameterType = propertyToTypeName(parameter['schema'])
                parameterName = parameter['name']

                functionParams += f'const {parameterType}& {sanitizeName(parameterName)}, '
                functionParamArgs += f'''
        Args.Add(TEXT("{parameterName}"), SanitizeArg(LexToString({sanitizeName(parameterName)})));'''
                
            if parameter['in'] == 'query':

                parameterType = propertyToTypeName(parameter['schema'])
                parameterName = parameter['name']

                functionParams += f'const {parameterType}& {sanitizeName(parameterName)}, '
                functionParamPath += f'{"&" if functionParamPath else "?"}{parameterName}={{{parameterName}}}'

                if propertyIsArray(parameter['schema']):
                    functionParamArgs += f'''
        Args.Add(TEXT("{parameterName}"), QueryListArgs(TEXT("{parameterName}"), {sanitizeName(parameterName)}));'''
                else:
                    functionParamArgs += f'''
        Args.Add(TEXT("{parameterName}"), SanitizeArg(LexToString({sanitizeName(parameterName)})));'''
        
        httpVerb = operationName.upper()
        httpPath = f'{pathName}{functionParamPath}'

        apiData += f'''
    /** {operationDesc} */
    static TWebPromisePtr<FResponseData<{returnType}>> {functionName}(
        const FDTHttpContext& Context, {functionParams}
        const {bodyType}& RequestBody)
    {{
        TMap<FString, FStringFormatArg> Args;{functionParamArgs}

        return UDTHttpRequester::SendHttpRequest<{bodyType}, {returnType}>(
            Context, "{httpVerb}", FString::Format(TEXT("{httpPath}"), Args), RequestBody);
    }}
'''

apiData += f'''}};'''

apiTargetPath = os.path.join(targetPath, "DTApi.h")
f = open(apiTargetPath, "w")
f.write(apiData)
f.close()

print(f'Generated API to {apiTargetPath}')
