
namespace hd {
class HD_NodeGraphManager {
    public:
        void ConnectNodes(const std::shared_ptr<HD_Node>& source, const std::shared_ptr<HD_Node>& target) {
            // Validate connection based on metadata
            if (IsValidConnection(source, target)) {
                connections.push_back({source, target});
            }
        }
    
        void ProcessGraph() {
            for (const auto& connection : connections) {
                connection.source->Update();
                connection.target->Update();
            }
        }
    
    private:
        struct Connection {
            std::shared_ptr<HD_Node> source;
            std::shared_ptr<HD_Node> target;
        };
    
        std::vector<Connection> connections;
    
        bool IsValidConnection(const std::shared_ptr<HD_Node>& source, const std::shared_ptr<HD_Node>& target) {
            // Validate based on metadata
            return true;  // Replace with actual validation logic
        }
    };
} // namespace hd
