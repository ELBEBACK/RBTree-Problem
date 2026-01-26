#pragma once
 
#include <iostream>

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

        Node_t(const T& key, Colour colour, Node_t* nil) : 
                    key_(key), 
                    left_(nil), 
                    right_(nil), 
                    parent_(nil), 
                    colour_(colour) {}

    };

    Node_t* NIL_;
    Node_t* root_;

    
    Node_t* nil_create() {
        Node_t* nil_node = reinterpret_cast<Node_t*>(::operator new(sizeof(Node_t)));
        nil_node->colour_ = BLACK;
        nil_node->left_ = nil_node;
        nil_node->right_ = nil_node;
        nil_node->parent_ = nil_node;
        return nil_node;
    }


    void left_rotate(Node_t *node) {
        auto new_local_root = node->right_;
        node->right_ = new_local_root->left_;

        if (new_local_root->left_ != NIL_) {
            new_local_root->left_->parent_ = node;
        }
        new_local_root->parent_ = node->parent_;

        if (node->parent_ == NIL_) {
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
        node->left_ = new_local_root->right_;

        if (new_local_root->right_ != NIL_) {
            new_local_root->right_->parent_ = node;
        }
        new_local_root->parent_ = node->parent_;

        if (node->parent_ == NIL_) {
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
        while (node->parent_ != NIL_ && node->parent_->colour_ == RED) {
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


    Node_t* search(const T& key) const {
        auto current = root_;
        while (current != NIL_) {
            if (key == current->key_) {
                return current;
            } else if (key < current->key_) {
                current = current->left_;
            } else {
                current = current->right_;
            }
        }
        return NIL_;
    }


    void transplant(Node_t* dst, Node_t* src) {
        if (dst->parent_ == NIL_) {
            root_ = src;
        } else if (dst == dst->parent_->left_) {
            dst->parent_->left_ = src;
        } else {
            dst->parent_->right_ = src;
        }
        if (src != NIL_)
            src->parent_ = dst->parent_;
    }


    void deletion_fix(Node_t* node) {
        while (node != root_ && node->colour_ == BLACK) {
            if (node == node->parent_->left_) {
                Node_t* sibling = node->parent_->right_;
                if (sibling->colour_ == RED) {
                    sibling->colour_ = BLACK;
                    node->parent_->colour_ = RED;
                    left_rotate(node->parent_);
                    sibling = node->parent_->right_;
                }
                if (sibling->left_->colour_ == BLACK && sibling->right_->colour_ == BLACK) {
                    sibling->colour_ = RED;
                    node = node->parent_;
                } else {
                    if (sibling->right_->colour_ == BLACK) {
                        sibling->left_->colour_ = BLACK;
                        sibling->colour_ = RED;
                        right_rotate(sibling);
                        sibling = node->parent_->right_;
                    }
                    sibling->colour_ = node->parent_->colour_;
                    node->parent_->colour_ = BLACK;
                    sibling->right_->colour_ = BLACK;
                    left_rotate(node->parent_);
                    node = root_;
                }
            } else {
                Node_t* sibling = node->parent_->left_;
                if (sibling->colour_ == RED) {
                    sibling->colour_ = BLACK;
                    node->parent_->colour_ = RED;
                    right_rotate(node->parent_);
                    sibling = node->parent_->left_;
                }
                if (sibling->right_->colour_ == BLACK && sibling->left_->colour_ == BLACK) {
                    sibling->colour_ = RED;
                    node = node->parent_;
                } else {
                    if (sibling->left_->colour_ == BLACK) {
                        sibling->right_->colour_ = BLACK;
                        sibling->colour_ = RED;
                        left_rotate(sibling);
                        sibling = node->parent_->left_;
                    }
                    sibling->colour_ = node->parent_->colour_;
                    node->parent_->colour_ = BLACK;
                    sibling->left_->colour_ = BLACK;
                    right_rotate(node->parent_);
                    node = root_;
                }
            }
        }
        node->colour_ = BLACK;
    }


    Node_t* minimum(Node_t* node) {
        while (node->left_ != NIL_) {
            node = node->left_;
        }
        return node;
    }


    void clear(Node_t* node) {
        if (node == NIL_) return; 
        
        clear(node->left_);
        clear(node->right_);
        
        delete node;
    }


public:
    RBTree_t() : NIL_(nil_create()), root_(NIL_) {}
    ~RBTree_t() {
        clear(root_);
        ::operator delete(NIL_);
    }
    
    RBTree_t(const RBTree_t&) = delete;
    RBTree_t& operator=(const RBTree_t&) = delete;

    RBTree_t(RBTree_t&&) = delete;
    RBTree_t& operator=(RBTree_t&&) = delete;


    void insert(const T& key) {
        auto new_node = new Node_t(key, RED, NIL_);
        auto current = root_;
        Node_t* parent = NIL_;

        while (current != NIL_) {
            parent = current;
            if (new_node->key_ < current->key_) {
                current = current->left_;
            }
            else {
                current = current->right_;
            }
        }

        new_node->parent_ = parent;
        if (parent == NIL_) {
            root_ = new_node;
        } else if (new_node->key_ < parent->key_) {
            parent->left_ = new_node;
        } else {
            parent->right_ = new_node;
        }

        insertion_fix(new_node);
    }

    
    void delete_node(const T& key) {
        auto target = search(key);
        if (target == NIL_) {
            std::cout << "There is no such value found in the tree.\n";
            return;
        }
        auto to_remove = target;
        auto fixup_node = NIL_;
        Colour original_colour = to_remove->colour_;
        if (target->left_ == NIL_) {
            fixup_node = target->right_;
            transplant(target, target->right_);
        } else if (target->right_ == NIL_) {
            fixup_node = target->left_;
            transplant(target, target->left_);
        } else {
            to_remove = minimum(target->right_);
            original_colour = to_remove->colour_;
            fixup_node = to_remove->right_;
            if (to_remove->parent_ == target) {
                fixup_node->parent_ = to_remove;
            } else {
                transplant(to_remove, to_remove->right_);
                to_remove->right_ = target->right_;
                to_remove->right_->parent_ = to_remove;
            }
            transplant(target, to_remove);
            to_remove->left_ = target->left_;
            to_remove->left_->parent_ = to_remove;
            to_remove->colour_ = target->colour_;
        }

        delete target;
        if (original_colour == BLACK) 
            deletion_fix(fixup_node);
    }   


    bool contains(const T& key) const {
        return search(key) != NIL_;
    }
};


}