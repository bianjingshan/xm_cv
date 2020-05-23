#pragma once


typedef struct XM_Node
{
	int count;
	void *p_data;
    struct XM_Node *p_branch;
    struct XM_Node *p_next;
}XM_NODE, *XM_PNODE;

XM_PNODE XMListCreate(void);
XM_PNODE XMListInsert(XM_PNODE p_node, void *p_data, int data_len);
XM_PNODE XMListInsertUnrepeatString(XM_PNODE branch_head, char *p_string);
bool XMListDelete(XM_PNODE p_node);
bool XMListDestroy(XM_PNODE p_head);



