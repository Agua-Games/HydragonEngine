class BloomNodeExample : public Node {
public:
    BloomNode() {
        // Property definitions with their metadata
        DefineProperty("intensity", 1.0f)
            .range(0.0f, 5.0f)
            .description("Bloom effect intensity")
            .category("Appearance");

        DefineProperty("threshold", 1.0f)
            .range(0.0f, 10.0f)
            .description("Brightness threshold for bloom effect")
            .category("Appearance");
    }
};