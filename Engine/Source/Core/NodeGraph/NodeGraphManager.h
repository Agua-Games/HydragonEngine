
namespace hd {
class HD_NodeGraphManager {
    public:
        bool ConnectNodes(const std::shared_ptr<HD_Node>& source, 
                         const std::shared_ptr<HD_Node>& target,
                         const std::string& sourcePort = "",
                         const std::string& targetPort = "") {
            // Validate connection based on metadata and port validation
            if (!IsValidConnection(source, target, sourcePort, targetPort)) {
                return false;
            }
            
            connections.push_back({source, target, sourcePort, targetPort});
            return true;
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
            std::string sourcePort;
            std::string targetPort;
        };

        std::vector<Connection> connections;

        bool IsValidConnection(const std::shared_ptr<HD_Node>& source, 
                             const std::shared_ptr<HD_Node>& target,
                             const std::string& sourcePort,
                             const std::string& targetPort) {
            // Type compatibility check
            if (!sourcePort.empty() && !targetPort.empty()) {
                if (!source->IsPortValid(sourcePort) || !target->IsPortValid(targetPort)) {
                    return false;
                }
            }

            // Cycle detection
            if (WouldCreateCycle(source, target)) {
                return false;
            }

            return true;
        }

        bool WouldCreateCycle(const std::shared_ptr<HD_Node>& source,
                             const std::shared_ptr<HD_Node>& target) {
            // Simple DFS to detect cycles
            std::unordered_set<HD_Node*> visited;
            return CheckCycle(target.get(), source.get(), visited);
        }

        bool CheckCycle(HD_Node* current, HD_Node* target, 
                       std::unordered_set<HD_Node*>& visited) {
            if (current == target) return true;
            if (visited.contains(current)) return false;
            
            visited.insert(current);
            for (const auto& conn : connections) {
                if (conn.source.get() == current) {
                    if (CheckCycle(conn.target.get(), target, visited)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };
} // namespace hd
