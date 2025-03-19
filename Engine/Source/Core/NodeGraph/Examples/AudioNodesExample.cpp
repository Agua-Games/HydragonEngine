/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

 #if 0
// Audio mixing/processing
auto audio = graph.create<AudioMixerNode>("gameMixer")
    .connect<AudioTrackNode>("music")
        .clip("audio/music.ogg")
        .volume(0.8f)
        .loop(true)
        .connect<ReverbNode>("reverb")
            .roomSize(0.8f)
            .damping(0.5f)
            .wetLevel(0.3f)
        .connect<CompressorNode>("compressor")
            .threshold(-12.0f)
            .ratio(4.0f)
            .attack(0.01f)
            .release(0.1f)
    .connect<AudioTrackNode>("sfx")
        .volume(1.0f)
        .connect<EQNode>("eq")
            .lowcut(20.0f)
            .highcut(20000.0f)
            .addBand(100.0f, 1.0f, 0.7f)
    .connect<SideChainNode>("duckMusic")
        .source("sfx")
        .target("music")
        .amount(0.5f)
        .attack(0.1f)
        .release(0.2f);

#endif