// Node.h
//
// Core classes for the nodal architecture: hd::Object (pure logical base)
// and hd::Node (base unit of connectivity, properties, and hierarchy).

#pragma once
#include "USDStubs.h"
#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace hd {

// Object: Logical base class for all reflected entities.
// Support for properties and runtime capability checks.
class Object : public std::enable_shared_from_this<Object> {
public:
  virtual ~Object() = default;

  template <typename T>
  void setLocalProperty(const std::string &name, const T &value) {
    m_localProperties[name] = value;
  }

  template <typename T>
  T getLocalProperty(const std::string &name,
                     const T &defaultValue = T()) const {
    auto it = m_localProperties.find(name);
    if (it != m_localProperties.end()) {
      try {
        return std::any_cast<T>(it->second);
      } catch (...) {
      }
    }
    return defaultValue;
  }

private:
  std::unordered_map<std::string, std::any> m_localProperties;
};

// Node: Base unit of the connectable node graph.
// Mirrors a pxr::UsdPrim for data authority. Does not have spatial
// transformation.
class Node : public Object {
public:
  struct Connection {
    std::shared_ptr<Node> target;
    std::string port;
  };

  Node(const std::string &name = "");
  virtual ~Node() override = default;

  // Hierarchy Management
  void addChild(std::shared_ptr<Node> child);
  void removeChild(std::shared_ptr<Node> child);
  std::shared_ptr<Node> getParent() const;
  const std::vector<std::shared_ptr<Node>> &getChildren() const {
    return m_children;
  }

  // Metadata
  std::string getName() const { return m_name; }
  void setName(const std::string &name) { m_name = name; }

  // USD Binding (Data Authority)
  void bindUsdPrim(const pxr::UsdPrim &prim);
  pxr::UsdPrim getUsdPrim() const { return m_usdPrim; }

  // Property System (Authority routed directly to USD)
  template <typename T>
  void setProperty(const std::string &name, const T &value) {
    if (m_usdPrim) {
      m_usdPrim.SetAttribute<T>(name, value);
    } else {
      setLocalProperty<T>(name, value);
    }
  }

  template <typename T>
  T getProperty(const std::string &name, const T &defaultValue = T()) const {
    if (m_usdPrim) {
      return m_usdPrim.GetAttribute<T>(name, defaultValue);
    }
    return getLocalProperty<T>(name, defaultValue);
  }

  // Direct Connect (Fluent style for node-in-code networks)
  template <typename T>
  std::shared_ptr<T> connect(std::shared_ptr<T> target,
                             const char *port = nullptr) {
    m_connections.push_back({target, port ? port : ""});
    return target;
  }

  const std::vector<Connection> &getConnections() const {
    return m_connections;
  }

  // Node Processing / Graph Evaluation
  virtual void processNode() {}

protected:
  std::string m_name;
  std::weak_ptr<Node> m_parent;
  std::vector<std::shared_ptr<Node>> m_children;
  std::vector<Connection> m_connections;

  pxr::UsdPrim m_usdPrim;
};

} // namespace hd
