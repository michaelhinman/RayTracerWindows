#pragma once
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <set>

#include "types.h"

namespace RT {
    namespace core {

        // forward declarations
        class Node;

        // Friend function of \see Node class allowing access to
        // class' protected destructor; to be passed to shared_ptr
        // constructor when instantiating a Node instance
        void NodeDeleter(Node* node);

#define RT_NODE_BASE(name) \
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW \
  using Ptr = std::shared_ptr<name>; \
  using ConstPtr = std::shared_ptr<name const>; \
  using WeakPtr = std::weak_ptr<name>; \
  template<typename... Args> \
  static std::shared_ptr<name> Create(Args... args) { \
    return Ptr(new name(args...), NodeDeleter);} \
  std::shared_ptr<name> GetPtr() { \
    return std::dynamic_pointer_cast<name>(shared_from_this());} \
  std::shared_ptr<name const> GetPtr() const { \
    return std::dynamic_pointer_cast<name const>(shared_from_this());}

#define RT_NODE(name) \
  RT_NODE_BASE(name) \
  std::shared_ptr<Node> Clone() const override { \
    return std::shared_ptr<Node>(new name(*this), NodeDeleter);}

        class Node : public std::enable_shared_from_this<Node> {
        public:
            RT_NODE_BASE(Node)
                using NodeMask = uint;
            using NodePath = std::vector<Node::Ptr>;

            // Constructor
            explicit Node(const std::string& name = std::string());

            // Move constructor
            Node(Node&& other) = delete;
            Node& operator=(const Node&) = delete;
            Node& operator=(Node&&) = delete;

            // Clone node.
            virtual std::shared_ptr<Node> Clone() const;

            // Set node name
            virtual void SetName(const std::string& name);

            // Get node's unique id
            virtual size_t GetGlobalNodeId() const final { return global_node_id_; }

            // Get node name
            virtual std::string GetName() const { return name_; }

            //        Friend function of \see Node class allowing access to
            //        class' protected destructor; to be passed to shared_ptr
            //        constructor when instantiating a Node instance

            friend void NodeDeleter(Node* node);
        protected:
            // Copy constructor to be used only by Clone()
            Node(const Node& other);

            // Destructor; made protected to prevent allocation of Node
            //                  on the stack or via new/malloc.
            virtual ~Node();

            // protected data members
            size_t global_node_id_{ 0 };  // unique node id
            std::string name_;          // node name

            // static data members
            static size_t node_count_;  // total number of nodes ever created
            static std::mutex node_count_mutex_;  // mutex used to set unique node id
        private:
            // Private function called by constructor to assign a
            //        unique id to each node
            void SetGlobalNodeId();
        };


        // less than operator comparing two nodes
        bool operator<(const Node::Ptr& a, const Node::Ptr& b);

        // less than operator comparing two nodes
        bool operator<(const Node::WeakPtr& a, const Node::WeakPtr& b);

    }  // namespace core
}  // namespace RT
