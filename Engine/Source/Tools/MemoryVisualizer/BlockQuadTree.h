#pragma once
#include <QRectF>
#include <memory>
#include <array>

namespace Hydragon {
namespace Tools {

class BlockQuadTree {
public:
    struct Node {
        QRectF bounds;
        bool occupied = false;
        std::array<std::unique_ptr<Node>, 4> children;
        
        // Metadata for visualization
        QString blockName;
        double utilizationRatio;
    };
    
    explicit BlockQuadTree(const QRectF& bounds, int maxDepth = 8);
    
    // Core operations
    Node* insert(const QRectF& rect);
    bool remove(const QRectF& rect);
    Node* findSpace(double size);
    
    // Visualization support
    void forEachNode(const std::function<void(const Node&)>& callback) const;
    std::vector<Node*> findOverlappingNodes(const QRectF& rect);

private:
    std::unique_ptr<Node> m_Root;
    int m_MaxDepth;
    
    Node* findSpaceInNode(Node* node, double size, int depth);
    bool subdivide(Node* node);
};

}} // namespace Hydragon::Tools 