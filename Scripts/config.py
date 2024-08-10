
## [C Type, C Setup Type, Copy-by-value]
typeAssignments = {
    "Boolean":              ["bool",                    "bool",                     True],
    "Number":               ["float",                   "float",                    True],
    "String":               ["FString",                 "FString",                  False],
    "SceneReg":             ["FDTSceneReg",             "FDTSceneRegSetup",         False],
    "EntityRef":            ["FDTEntityRef",            "FDTEntityRefSetup",        False],
    "LevelAsset":           ["FDTLevelAsset",           "FDTLevelAsset",            False],
    "ActorAsset":           ["FDTActorAsset",           "FDTActorAsset",            False],
    "StaticMeshAsset":      ["FDTStaticMeshAsset",      "FDTStaticMeshAsset",       False],
    "SkeletalMeshAsset":    ["FDTSkeletalMeshAsset",    "FDTSkeletalMeshAsset",     False],
    "TextureAsset":         ["FDTTextureAsset",         "FDTTextureAsset",          False],
    "AnimationAsset":       ["FDTAnimationAsset",       "FDTAnimationAsset",        False],
    "Transform":            ["FDTTransform",            "FDTTransform",             False],
    "BooleanArray":         ["TArray<bool>",            "TArray<bool>",             False],
    "NumberArray":          ["TArray<float>",           "TArray<float>",            False],
    "StringArray":          ["TArray<FString>",         "TArray<FString>",          False],
    "Type":                 ["EDTType",                 "EDTType",                  True],
}

## [Asset Type, C Type]
assetAssignments = {
    "Factory":      ["UDTGameFactoryAsset*",    "type.factory"],
    "Level":        ["TSoftObjectPtr<UWorld>",  "type.entity.world"],
    "Actor":        ["TSubclassOf<AActor>",     "type.entity.actor"],
    "StaticMesh":   ["UStaticMesh*",            "type.mesh.static"],
    "SkeletalMesh": ["USkeletalMesh*",          "type.mesh.skeletal"],
    "Texture":      ["UTexture*",               "type.texture"],
    "Animation":    ["UAnimationAsset*",        "type.animation"],
}

def parameterize(prop, type):
    if prop in typeAssignments and typeAssignments[prop][2]:
        return f'{type}'
    return f'const {type}&'

def getPropCType(prop, isParam = False):
    typeName = "FString"
    if prop in typeAssignments:
        typeName = typeAssignments[prop][0]
    else:
        print(f'No C++ type for property {prop} defined, please add it to the config file. Defaulting to FString.')
    return parameterize(prop, typeName) if isParam else typeName

def getPropCSetup(prop, isParam = False):
    typeName = "FString"
    if prop in typeAssignments:
        typeName = typeAssignments[prop][1]
    else:
        print(f'No C++ setup for property {prop} defined, please add it to the config file. Defaulting to FString.')
    return parameterize(prop, typeName) if isParam else typeName

def getAssetCType(type):
    typeName = "FString"
    if type in assetAssignments:
        typeName = assetAssignments[type][0]
    else:
        print(f'No C++ type for asset type {type} defined, please add it to the config file. Defaulting to FString.')
    return typeName

def getAssetTag(type):
    typeName = "FString"
    if type in assetAssignments:
        typeName = assetAssignments[type][1]
    else:
        print(f'No C++ tag for asset type {type} defined, please add it to the config file. Defaulting to FString.')
    return typeName