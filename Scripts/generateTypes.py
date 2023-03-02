import json
import sys
import os 

from config import getPropCType, getPropCSetup

dir_path = os.path.dirname(os.path.realpath(__file__))

sourcePath = os.path.join(dir_path, "../NodeJS/Server/build/generated", "types.json")
targetPath = os.path.join(dir_path, "../Source/DTHttpClient/Public/Generated")

if not os.path.exists(sourcePath):
    sys.exit(f'Source path /"{sourcePath}/" not found')

sourceFile = open(sourcePath)
sourceJson = json.load(sourceFile)

def sanitizeName(name):
    if len(name) == 0: return ""
    return name[0].upper() + name[1:]


# Print settings
typesData = f'''
#pragma once

#include "CoreMinimal.h"
#include "DTTypes.generated.h"
'''

typesData += f'''
UENUM(BlueprintType)
enum class EDTType : uint8
{{
    Invalid'''
for type in sourceJson:
    typesData += f''',
    {type['typeKey']}'''
typesData += f'''
}};

FORCEINLINE FString LexToString(EDTType Value)
{{
    switch (Value)
    {{'''
for type in sourceJson:
    typesData += f'''
    case EDTType::{type['typeKey']}: return TEXT("{type['typeKey']}");'''
typesData += f'''
    default: return TEXT("");
    }}
}};

FORCEINLINE void LexFromString(EDTType& Value, const TCHAR* Buffer)
{{
    FString Name = FString(Buffer);'''
for type in sourceJson:
    typesData += f'''
    if (Name.Equals(TEXT("{type['typeKey']}"))) Value = EDTType::{type['typeKey']}; else'''
typesData += f'''
    {{ Value = EDTType::Invalid; }}
}};
'''

typesTargetPath = os.path.join(targetPath, "DTTypes.h")
f = open(typesTargetPath, "w")
f.write(typesData)
f.close()

print(f'Generated Settings to {typesTargetPath}')

# Print types
objectsData = f'''
#pragma once

#include "CoreMinimal.h"
#include "DTTypes.h"
#include "Game/DTGameBaseObject.h"
#include "Engine/DeveloperSettings.h"
#include "DTObjects.generated.h"
'''

for type in sourceJson:

    parent = type['parent'] if type['parent'] else 'Base'
    objectsData += f'''
UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGame{type['typeKey']}Object : public UDTGame{parent}Object
{{
	GENERATED_BODY()
public:
'''
    # Write timeline properties
    for prop in type['props']:

        if prop['isPublic']:
            cType = getPropCType(prop['type'], False)
            propKey = sanitizeName(prop['propKey'])
            objectsData += f'''
    FDTGenericTimeline<{cType}> {propKey}Property = "{prop['initial']}";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void Get{propKey}(const FDateTime& Timestamp, {cType}& Previous, {cType}& Value, float& Alpha) const
        {{
            const FDTGenericKeyframe<{cType}>& keyframe = {propKey}Property.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }}
'''
    # Write attributes
    for attr in type['attrs']:

        if attr['isPublic']:
            propKey = sanitizeName(attr['propKey'])
            objectsData += f'''
    
    FDTGenericPropertyValue<{getPropCType(attr['type'], False)}> {propKey}Property = "{attr['initial']}";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        {getPropCType(attr['type'], True)} Get{propKey}() const
        {{
            return {propKey}Property.Value;
        }}
'''

    objectsData += f'''
	virtual bool PushChange(const FDTPropertyChange& Change) override
	{{'''

    # Write property switch for push
    for prop in type['props']:

        if prop['isPublic']:
            objectsData += f'''
		if (Change.PropReg.Equals(TEXT("{prop['propKey']}"))) {{ {sanitizeName(prop['propKey'])}Property.Push(Change); return true; }}'''

    objectsData += f'''
		return Super::PushChange(Change);
	}}'''

    objectsData += f'''
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{{'''

    # Write property switch for push
    for attr in type['attrs']:

        if attr['isPublic']:
            objectsData += f'''
		if (data.PropReg.Equals(TEXT("{attr['propKey']}"))) {{ LexFromString({sanitizeName(attr['propKey'])}Property.Value, *data.PropVal); return true; }}'''

    objectsData += f'''
		return Super::PushAttributes(data);
	}}'''
    
    objectsData += f'''
	virtual EDTType GetType() const override
    {{
        return EDTType::{type['typeKey']};
    }}
}};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGame{type['typeKey']}Tick, UDTGame{type['typeKey']}Object*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGame{type['typeKey']}Update, UDTGame{type['typeKey']}Object*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGame{type['typeKey']}Component : public UDTGameBaseComponent
{{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGame{type['typeKey']}Object> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGame{type['typeKey']}Tick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGame{type['typeKey']}Update OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{{
		GameObject = Cast<UDTGame{type['typeKey']}Object>(InGameObject);
		return GameObject.IsValid();
	}}

	virtual void GameTick(const FDateTime& Timestamp) override
	{{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {{
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }}
	}}

	virtual void GameUpdate() override
	{{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {{
            OnGameUpdate.Broadcast(GameObject.Get());
        }}
	}}
}};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetup{type['typeKey']}Actor : public ADTSetup{parent}Actor
{{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void Get{type['typeKey']}SetupValues(ADTSceneActor* ParentScene'''
    for prop in type['props'] + type['attrs']:

        objectsData += f'''
        , {getPropCSetup(prop['type'], False)}& {sanitizeName(prop['propKey'])}'''

    objectsData += f''') const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{{
		Super::GatherSetupProps(ParentScene, Attrs, Props);'''
    
    for prop in type['props'] + type['attrs']:

        objectsData += f'''
        {getPropCSetup(prop['type'], False)} {sanitizeName(prop['propKey'])};'''

    objectsData += f'''
        Get{type['typeKey']}SetupValues(ParentScene'''
    
    for prop in type['props'] + type['attrs']:

        objectsData += f''', {sanitizeName(prop['propKey'])}'''
        
    objectsData += f''');'''
    
    for prop in type['props']:

        objectsData += f'''
        Props.Emplace(TEXT("{prop['propKey']}"), LexToString({sanitizeName(prop['propKey'])}));'''

    for prop in type['attrs']:

        objectsData += f'''
        Attrs.Emplace(TEXT("{prop['propKey']}"), LexToString({sanitizeName(prop['propKey'])}));'''
    
    
    objectsData += f'''
		return TEXT("{type['typeKey']}");
	}}
}};
'''

objectsData += f'''
FORCEINLINE TSubclassOf<UDTGameBaseObject> GetTypeObjectClass(EDTType Type)
{{
    switch(Type)
    {{'''

for type in sourceJson:

    objectsData += f'''
        case EDTType::{type['typeKey']}: return UDTGame{type['typeKey']}Object::StaticClass();'''

objectsData += f'''
        default: return nullptr;
    }}
}}
''' 

objectsTargetPath = os.path.join(targetPath, "DTObjects.h")
f = open(objectsTargetPath, "w")
f.write(objectsData)
f.close()

print(f'Generated Objects to {objectsTargetPath}')