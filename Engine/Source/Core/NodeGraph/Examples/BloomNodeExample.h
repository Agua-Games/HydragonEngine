/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Bloom effect node with intensity and threshold
 */

 #if 0
class BloomNodeExample : public Node {
public:
    BloomNodeExample() {
        // Property definitions with their metadata
        defineProperty("intensity", 1.0f)
            .range(0.0f, 5.0f)
            .description("Bloom effect intensity")
            .category("Appearance");

        defineProperty("threshold", 1.0f)
            .range(0.0f, 10.0f)
            .description("Brightness threshold for bloom effect")
            .category("Appearance");
    }
};
#endif