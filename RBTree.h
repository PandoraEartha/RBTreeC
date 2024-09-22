#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct RBTreeNode{
	struct RBTreeNode* father;
	struct RBTreeNode* left;
	struct RBTreeNode* right;
	long long unsigned int key;
	long long unsigned int value;
	char color;
}Node;

typedef struct RBTreeHead{
	long long unsigned int nodeCount;
	Node* root;
}Head;

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

#define ROUTE_CASE_RR 0
#define ROUTE_CASE_RL 1
#define ROUTE_CASE_LR 2
#define ROUTE_CASE_LL 3

#define COLOR_RED 0
#define COLOR_BLACK 1

static inline Head* RBTreeCreate(){
	Head* tree=(Head*)malloc(sizeof(Head));
	if(tree){
		memset(tree,0,sizeof(Head));
	}
	return tree;
}

static inline Node* RBTreeNodeCreate(Head* tree,Node* father,long long unsigned int key,long long unsigned int value){
	Node* newNode=NULL;
	newNode=(Node*)malloc(sizeof(Node));
	if(newNode){
		memset(newNode,0,sizeof(Node));
		newNode->father=father;
		newNode->key=key;
		newNode->value=value;
		tree->nodeCount=tree->nodeCount+1;
		return newNode;
	}
	return NULL;
}

static inline unsigned RBTreeNodeGetRouteCase(const Node* current,const Node* father,const Node* grandfather){
	if(grandfather->left==father){
		if(father->left==current){
			return ROUTE_CASE_LL;
		}
		return ROUTE_CASE_LR;
	}
	if(father->left==current){
		return ROUTE_CASE_RL;
	}
	return ROUTE_CASE_RR;
}

static inline bool RBTreeSearch(const Head* tree,long long unsigned int key,long long unsigned int* value){
	if((!tree)||(!value)){
		return false;
	}
	Node* current=tree->root;
	while(current){
		if(key>current->key){
			current=current->right;
			continue;
		}
		if(key<current->key){
			current=current->left;
			continue;
		}
		*(value)=current->value;
		return true;
	}
	return false;
}

static inline Node* RBTreeSearchNode(const Head* tree,long long unsigned int key){
	if(!tree){
		return NULL;
	}
	Node* current=tree->root;
	while(current){
		if(key>current->key){
			current=current->right;
			continue;
		}
		if(key<current->key){
			current=current->left;
			continue;
		}
		return current;
	}
	return NULL;
}

static inline bool RBTreeInsert(Head* tree,long long unsigned int key,long long unsigned int value){
	if(!tree){
		return false;
	}
	// printf("%llu: %llu\n",key,value);
	if(unlikely(!(tree->root))){
		tree->root=RBTreeNodeCreate(tree,NULL,key,value);
		tree->root->color=COLOR_BLACK;
		return true;
	}
	Node* current=tree->root;
	while(current){
		if(key>current->key){
			if(!(current->right)){
				current->right=RBTreeNodeCreate(tree,current,key,value);
				current=current->right;
				break;
			}
			current=current->right;
			continue;
		}
		if(key<current->key){
			if(!(current->left)){
				current->left=RBTreeNodeCreate(tree,current,key,value);
				current=current->left;
				break;
			}
			current=current->left;
			continue;
		}
		current->value=value;
		return true;
	}
	Node* father=current->father;
	Node* grandfather=current->father->father;
	Node* greatGrandfather=NULL;
	// RR==0 RL==1 LR==2 LL==3
	unsigned routeCase;
	if(grandfather){
		while(current->color==COLOR_RED&&current->father->color==COLOR_RED){
			routeCase=RBTreeNodeGetRouteCase(current,father,grandfather);
			switch(routeCase){
			case ROUTE_CASE_RR:
				if(grandfather->left){
					if(grandfather->left->color==COLOR_RED){
						goto redUncle;
					}
				}
				grandfather->right=father->left;
				if(grandfather->right){
					grandfather->right->father=grandfather;
				}
				father->left=grandfather;
				greatGrandfather=grandfather->father;
				grandfather->father=father;
				father->father=greatGrandfather;
				if(grandfather==tree->root){
					tree->root=father;
				}else{
					if(greatGrandfather->left==grandfather){
						greatGrandfather->left=father;
					}else{
						greatGrandfather->right=father;
					}
				}
				father->color=COLOR_BLACK;
				grandfather->color=COLOR_RED;
				return true;
			case ROUTE_CASE_RL:
				if(grandfather->left){
					if(grandfather->left->color==COLOR_RED){
						goto redUncle;
					}
				}
				father->left=current->right;
				if(father->left){
					father->left->father=father;
				}
				grandfather->right=current->left;
				if(grandfather->right){
					grandfather->right->father=grandfather;
				}
				current->left=grandfather;
				current->right=father;
				greatGrandfather=grandfather->father;
				current->father=greatGrandfather;
				father->father=current;
				grandfather->father=current;
				if(grandfather==tree->root){
					tree->root=current;
				}else{
					if(greatGrandfather->left==grandfather){
						greatGrandfather->left=current;
					}else{
						greatGrandfather->right=current;
					}
				}
				current->color=COLOR_BLACK;
				grandfather->color=COLOR_RED;
				return true;
			case ROUTE_CASE_LR:
				if(grandfather->right){
					if(grandfather->right->color==COLOR_RED){
						goto redUncle;
					}
				}
				father->right=current->left;
				if(father->right){
					father->right->father=father;
				}
				grandfather->left=current->right;
				if(grandfather->left){
					grandfather->left->father=grandfather;
				}
				current->left=father;
				current->right=grandfather; 
				greatGrandfather=grandfather->father;
				current->father=greatGrandfather;
				father->father=current;
				grandfather->father=current;
				if(grandfather==tree->root){
					tree->root=current;
				}else{
					if(greatGrandfather->left==grandfather){
						greatGrandfather->left=current;
					}else{
						greatGrandfather->right=current;
					}
				}
				current->color=COLOR_BLACK;
				grandfather->color=COLOR_RED;
				return true;
			case ROUTE_CASE_LL:
				if(grandfather->right){
					if(grandfather->right->color==COLOR_RED){
						goto redUncle;
					}
				}
				grandfather->left=father->right;
				if(grandfather->left){
					grandfather->left->father=grandfather;
				}
				father->right=grandfather;
				greatGrandfather=grandfather->father;
				father->father=greatGrandfather;
				grandfather->father=father;
				if(tree->root==grandfather){
					tree->root=father;
				}else{
					if(greatGrandfather->left==grandfather){
						greatGrandfather->left=father;
					}else{
						greatGrandfather->right=father;
					}
				}
				father->color=COLOR_BLACK;
				grandfather->color=COLOR_RED;
				return true;
			default:
				return false;
			}
		redUncle:
			grandfather->color=COLOR_RED;
			grandfather->left->color=COLOR_BLACK;
			grandfather->right->color=COLOR_BLACK;
			current=current->father->father;
			if(current==tree->root||current->father==tree->root){
				tree->root->color=COLOR_BLACK;
				return true;
			}
			grandfather=current->father->father;
			father=current->father;
		}
	}
	return true;
}

static inline bool RBTreeDelete(Head* tree,long long unsigned int key){
	if(!tree){
		return false;
	}
	Node* current=tree->root;
	if(unlikely(!current)){
		return false;
	}
	if(unlikely(tree->nodeCount==1)){
		if(key==current->key){
			tree->root=NULL;
			free(current);
			tree->nodeCount=0;
			return true;
		}
		return false;
	}
	while(current){
		if(key>current->key){
			if(unlikely(!(current->right))){
				return false;
			}
			current=current->right;
			continue;
		}
		if(key<current->key){
			if(unlikely(!(current->left))){
				return false;
			}
			current=current->left;
			continue;
		}
		break;
	}
	bool deleted=false;
	Node* brother;
	Node* father=current->father;
	Node* grandfather;
	if(unlikely(!father)){
		if(!(current->left)){
			current->key=current->right->key;
			current->value=current->right->value;
			free(current->right);
			current->right=NULL;
			tree->nodeCount=1;
			return true;
		}else{
			if(!(current->right)){
				current->key=current->left->key;
				current->value=current->left->value;
				free(current->left);
				current->left=NULL;
				tree->nodeCount=1;
				return true;
			}else{
				goto gotoRightSmallest;
			}
		}
	}
	if(!(current->left)){
		deleteBegin:
		if(!(current->right)){
			// No child
			if(current->color==COLOR_RED){
				if(father->left==current){
					father->left=NULL;
				}else{
					father->right=NULL;
				}
				free(current);
				tree->nodeCount=tree->nodeCount-1;
				return true;
			}
			doubleBlackFix:
			grandfather=father->father;
			if(father->left==current){
				brother=father->right;
				if(brother->color==COLOR_BLACK){
					if(brother->right){
						if(brother->right->color==COLOR_RED){
							// case RR
							if(!deleted){
								father->left=NULL;
								free(current);
								tree->nodeCount=tree->nodeCount-1;
							}
							brother->right->color=COLOR_BLACK;
							brother->color=father->color;
							father->color=COLOR_BLACK;
							father->right=brother->left;
							if(father->right){
								father->right->father=father;
							}
							brother->left=father;
							father->father=brother;
							brother->father=grandfather;
							if(tree->root==father){
								tree->root=brother;
							}else{
								if(grandfather->left==father){
									grandfather->left=brother;
								}else{
									grandfather->right=brother;
								}
							}
							return true;
						}
					}
					if(brother->left){
						if(brother->left->color==COLOR_RED){
							// case RL
							if(!deleted){
								father->left=NULL;
								free(current);
								tree->nodeCount=tree->nodeCount-1;
							}
							Node* leftChild=brother->left;
							leftChild->color=father->color;
							father->color=COLOR_BLACK;
							father->right=leftChild->left;
							if(father->right){
								father->right->father=father;
							}
							leftChild->left=father;
							father->father=leftChild;
							leftChild->father=grandfather;
							if(tree->root==father){
								tree->root=leftChild;
							}else{
								if(grandfather->left==father){
									grandfather->left=leftChild;
								}else{
									grandfather->right=leftChild;
								}
							}
							brother->left=leftChild->right;
							if(brother->left){
								brother->left->father=brother;
							}
							leftChild->right=brother;
							brother->father=leftChild;
							return true;
						}
					}
					if(!deleted){
						father->left=NULL;
						free(current);
						tree->nodeCount=tree->nodeCount-1;
					}
					goto brotherChildBothBlack;
				}else{
					brother->color=COLOR_BLACK;
					father->color=COLOR_RED;
					father->right=brother->left;
					if(father->right){
						father->right->father=father;
					}
					father->father=brother;
					brother->left=father;
					brother->father=grandfather;
					if(tree->root==father){
						tree->root=brother;
					}else{
						if(grandfather->left==father){
							grandfather->left=brother;
						}else{
							grandfather->right=brother;
						}
					}
					goto doubleBlackFix;
				}
			}else{
				brother=father->left;
				if(brother->color==COLOR_BLACK){
					if(brother->left){
						if(brother->left->color==COLOR_RED){
							// case LL
							if(!deleted){
								father->right=NULL;
								free(current);
								tree->nodeCount=tree->nodeCount-1;
							}
							brother->left->color=COLOR_BLACK;
							brother->color=father->color;
							father->color=COLOR_BLACK;
							father->left=brother->right;
							if(father->left){
								father->left->father=father;
							}
							brother->right=father;
							father->father=brother;
							brother->father=grandfather;
							if(tree->root==father){
								tree->root=brother;
							}else{
								if(grandfather->left==father){
									grandfather->left=brother;
								}else{
									grandfather->right=brother;
								}
							}
							return true;
						}
					}
					if(brother->right){
						if(brother->right->color==COLOR_RED){
							// case LR
							if(!deleted){
								father->right=NULL;
								free(current);
								tree->nodeCount=tree->nodeCount-1;
							}
							Node* rightChild=brother->right;
							rightChild->color=father->color;
							father->color=COLOR_BLACK;
							brother->right=rightChild->left;
							if(brother->right){
								brother->right->father=brother;
							}
							rightChild->left=brother;
							brother->father=rightChild;
							rightChild->father=grandfather;
							if(tree->root==father){
								tree->root=rightChild;
							}else{
								if(grandfather->left==father){
									grandfather->left=rightChild;
								}else{
									grandfather->right=rightChild;
								}
							}
							father->left=rightChild->right;
							if(father->left){
								father->left->father=father;
							}
							rightChild->right=father;
							father->father=rightChild;
							return true;
						}
					}
					// both of the brother's childern are null or black, and brother is black
					if(!deleted){
						father->right=NULL;
						free(current);
						tree->nodeCount=tree->nodeCount-1;
					}
					brotherChildBothBlack:
					brother->color=COLOR_RED;
					if(unlikely(tree->root==father)){
						return true;
					}
					if(father->color==COLOR_RED){
						father->color=COLOR_BLACK;
						return true;
					}
					current=father;
					deleted=true;
					father=current->father;
					grandfather=father->father;
					goto doubleBlackFix;
				}else{
					brother->color=COLOR_BLACK;
					father->color=COLOR_RED;
					father->left=brother->right;
					if(father->left){
						father->left->father=father;
					}
					father->father=brother;
					brother->right=father;
					brother->father=grandfather;
					if(tree->root==father){
						tree->root=brother;
					}else{
						if(grandfather->left==father){
							grandfather->left=brother;
						}else{
							grandfather->right=brother;
						}
					}
					goto doubleBlackFix;
				}
			}
		}
		// no left child but right child
		current->key=current->right->key;
		current->value=current->right->value;
		free(current->right);
		current->right=NULL;
		tree->nodeCount=tree->nodeCount-1;
		return true;
	}else{
		if(!(current->right)){
			// no right child but left child
			current->key=current->left->key;
			current->value=current->left->value;
			free(current->left);
			current->left=NULL;
			tree->nodeCount=tree->nodeCount-1;
			return true;
		}
		// left child and right child
		gotoRightSmallest:
		Node* rightSmallest=current->right;
		while(rightSmallest->left){
			rightSmallest=rightSmallest->left;
		}
		current->key=rightSmallest->key;
		current->value=rightSmallest->value;
		current=rightSmallest;
		father=current->father;
		grandfather=father->father;
		goto deleteBegin;
	}
}

static void RBTreeClearRecursive(Node* current){
	if(current){
		RBTreeClearRecursive(current->left);
		RBTreeClearRecursive(current->right);
		free(current);
	}
}

static inline bool RBTreeClear(Head* tree){
	if(!tree){
		return false;
	}
	if(!tree->nodeCount){
		return true;
	}
	Node* current=tree->root;
	RBTreeClearRecursive(current);
	tree->nodeCount=0;
	return true;
}

static inline bool RBTreeDestroy(Head* tree){
	if(!tree){
		return false;
	}
	if(!tree->nodeCount){
		free(tree);
		return true;
	}
	RBTreeClear(tree);
	free(tree);
	return true;
}

static inline long long unsigned int RBTreeKeyCount(Head* tree){
	if(!tree){
		return 0;
	}
	return tree->nodeCount;
}

static inline void RBTreeGetMax(const Head* tree,long long unsigned int* key,long long unsigned int* value){
	if((!tree)||(!key)||(!value)){
		return;
	}
	if(!tree->nodeCount){
		return;
	}
	Node* current=tree->root;
	while(current->right){
		current=current->right;
	}
	*(key)=current->key;
	*(value)=current->value;
}

static inline void RBTreeGetMin(const Head* tree,long long unsigned int* key,long long unsigned int* value){
	if((!tree)||(!key)||(!value)){
		return;
	}
	if(!tree->nodeCount){
		return;
	}
	Node* current=tree->root;
	while(current->left){
		current=current->left;
	}
	*(key)=current->key;
	*(value)=current->value;
}

#endif