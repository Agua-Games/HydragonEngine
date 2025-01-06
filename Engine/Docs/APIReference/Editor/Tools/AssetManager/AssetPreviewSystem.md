
## Classes

### AssetPreviewSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AssetPreviewConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RequestPreview



**Parameters:** const std::string& assetPath

### CancelPreview



**Parameters:** const std::string& assetPath

### SetPreviewSize



**Parameters:** uint32_t size

### SetPreviewQuality



**Parameters:** PreviewQuality quality

### RegisterPreviewGenerator



**Parameters:** const std::string& assetType, 
                                std::unique_ptr<IPreviewGenerator> generator

### InvalidatePreview



**Parameters:** const std::string& assetPath

### ClearPreviewCache



**Parameters:** 
