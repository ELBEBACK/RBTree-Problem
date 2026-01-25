#pragma once

#include <cassert>


namespace rbtree {


enum Colour { BLACK, RED };

template <typename T>
class RBTree_t {

    struct Node_t {

        T key_;
        Node_t* left_;
        Node_t* right_;
        Node_t* parent_;
        Colour colour_;

        Node_t(const T& key, Colour colour = RED) : 
                    key_(key), 
                    left_(nullptr), 
                    right_(nullptr), 
                    parent_(nullptr), 
                    colour_(colour) {}

    };

    Node_t* root_;

public:
    RBTree_t() = default;
    ~RBTree_t() = default;

    void left_rotate(Node_t *node) {
        auto new_local_root = node->right_;
        node.right_ = new_local_root->left_;

        if (new_local_root->left_ != nullptr) {
            new_local_root->left_->parent_ = node;
        }
        new_local_root->parent_ = node->parent_;

        if (node->parent_ == nullptr) {
            root_ = new_local_root;
        }
        else if (node->parent_->left_ == node) {
            node->parent_->left_ = new_local_root;
        }
        else {
            node->parent_->right_ = new_local_root;
        }

        new_local_root->left_ = node;
        node->parent_ = new_local_root;
    }

    void right_rotate(Node_t *node) {
        auto new_local_root = node->left_;
        node.left_ = new_local_root->right_;

        if (new_local_root->right_ != nullptr) {
            new_local_root->right_->parent_ = node;
        }
        new_local_root->parent_ = node->parent_;

        if (node->parent_ == nullptr) {
            root_ = new_local_root;
        }
        else if (node->parent_->right_ == node) {
            node->parent_->right_ = new_local_root;
        }
        else {
            node->parent_->left_ = new_local_root;
        }

        new_local_root->right_ = node;
        node->parent_ = new_local_root;
    }

    
};


}