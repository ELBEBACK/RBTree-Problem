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


    void insertion_fix(Node_t* node) {
        while (node->parent_ != nullptr && node->parent_->colour_ == RED) {
            if (node->parent_ == node->parent_->parent_->left_) {
                Node_t* uncle = node->parent_->parent_->right_;
                if (uncle->colour_ == RED) {
                    node->parent_->colour_ = BLACK;
                    uncle->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    node = node->parent_->parent_;
                } else {
                    if (node == node->parent_->right_) {
                        node = node->parent_;
                        left_rotate(node);
                    }
                    node->parent_->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    right_rotate(node->parent_->parent_);
                }
            } else {
                Node_t* uncle = node->parent_->parent_->left_;
                if (uncle->colour_ == RED) {
                    node->parent_->colour_ = BLACK;
                    uncle->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    node = node->parent_->parent_;
                } else {
                    if (node == node->parent_->left_) {
                        node = node->parent_;
                        right_rotate(node);
                    }
                    node->parent_->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    left_rotate(node->parent_->parent_);
                }
            }
        }
        root_->colour_ = BLACK;
    }


    Node_t* search(const T& key) {
        auto current = root_;
        while (current != nullptr) {
            if (key == current->key_) {
                return current;
            } else if (key < current->key_) {
                current = current->left_;
            } else {
                current = current->right_;
            }
        }
        return nullptr;
    }


    void transplant(Node_t* dst, Node_t* src) {
        if (dst->parent_ == nullptr) {
            root_ = src;
        } else if (dst == dst->parent_->left_) {
            dst->parent_->left_ = src;
        } else {
            dst->parent_->right_ = src;
        }
        src->parent_ = dst->parent_;
    }


public:
    RBTree_t() = default;
    ~RBTree_t() = default;


    void insert(const T& key) {
        auto new_node = new Node_t(key);
        auto current = root_;
        Node_t* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (new_node->key_ < current->key_) {
                current = current->left_;
            }
            else {
                current = current->right_;
            }
        }

        new_node->parent_ = parent;
        if (parent == nullptr) {
            root_ = new_node;
        } else if (new_node->key_ < parent->key_) {
            parent->left_ = new_node;
        } else {
            parent->right_ = new_node;
        }

        insertion_fix(new_node);
    }


};


}