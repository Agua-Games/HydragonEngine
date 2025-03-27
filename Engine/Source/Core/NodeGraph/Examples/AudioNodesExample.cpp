/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 * 
 * These examples demonstrate:
 * 
 *  - Audio processing chain with reverb, compression, and EQ
 *  - Audio mixing with sidechain compression
 */

 #if 0
// Audio processing chain
auto audio = graph.create<AudioProcessor>("mainAudio")
    .connect<AudioClip>("music")
        .file("audio/music.ogg")
        .volume(0.8f)
        .looping(true)
        .connect<Reverb>("reverb")
            .roomSize(0.8f)
            .damping(0.5f)
            .wetLevel(0.3f)
        .connect<Compressor>("compressor")
            .threshold(-12.0f)
            .ratio(4.0f)
            .attack(0.01f)
            .release(0.1f)
    .connect<AudioClip>("sfx")
        .volume(1.0f)
        .connect<EQ>("eq")
            .lowcut(20.0f)
            .highcut(20000.0f)
            .addBand(100.0f, 1.0f, 0.7f)
    .connect<SideChain>("duckMusic")
        .source("sfx")
        .target("music")
        .amount(0.5f)
        .attack(0.1f)
        .release(0.2f);

#endif