#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <math.h> 
#include <stdio.h>
#include "common.h"

XM_PNODE XMListCreate(void)
{
    XM_PNODE p_head;

    p_head=(XM_PNODE)malloc(sizeof(XM_NODE));
	if(p_head==NULL)
	{
		return NULL;
	}
	p_head->count=0;
	p_head->p_data=NULL;
	p_head->p_next=NULL;
	p_head->p_branch=NULL;
	return p_head;
}

//insert到p_node后
XM_PNODE XMListInsert(XM_PNODE p_node, void *p_data, int data_len)
{
	if(p_node==NULL)
	{
		return NULL;
	}
	if(p_data==NULL)
	{
		return NULL;
	}
	if(data_len<=0)
	{
		return NULL;
	}
    XM_PNODE p_new_node=(XM_PNODE)malloc(sizeof(XM_NODE));
	if(p_new_node==NULL)
	{
		return NULL;
	}
	p_new_node->count=0;
	p_new_node->p_branch=NULL;
	p_new_node->p_data=malloc(data_len);
	memcpy(p_new_node->p_data, p_data, data_len);
	p_new_node->p_next=p_node->p_next;
	p_node->p_next=p_new_node;
	return p_new_node;
}


bool XMListDelete(XM_PNODE p_node)
{
	if(p_node==NULL)
	{
		return false;
	}
	if(p_node->p_data==NULL)
	{
		return false;
	}
	free(p_node->p_data);
	if(p_node->p_branch)
	{
        XMListDestroy(p_node->p_branch);
	}
	p_node->p_next=p_node->p_next->p_next;
	p_node->p_data=p_node->p_next->p_data;
	free(p_node->p_next);
	return true;
}


bool XMListDestroy(XM_PNODE p_head)
{
	if(p_head==NULL)
	{
		return false;
	}
    XM_PNODE cruent_node=p_head, next_node;
	while(cruent_node)
	{
		next_node=cruent_node->p_next;
		if(cruent_node->p_data)
		{
			free(cruent_node->p_data);
		}
		if(cruent_node->p_branch)
		{
            XMListDestroy(cruent_node->p_branch);
		}
		free(cruent_node);
		cruent_node=next_node;
	}
	return true;
}



XM_PNODE XMListInsertUnrepeatString(XM_PNODE branch_head, char *p_string)
{
    XM_PNODE p_current, p_pre, insert_pos;

	p_pre=branch_head;
	p_current=branch_head->p_next;
	int cmp_result;
	while(p_current)
	{
		cmp_result=strcmp(p_string, (char *)p_current->p_data);
		if(cmp_result==0)
		{
			return p_current;
		}
		else if(cmp_result<0)
		{
            return XMListInsert(p_pre, p_string, strlen(p_string)+1);
		}
		p_pre=p_current;
		p_current=p_current->p_next;
	}
    return XMListInsert(p_pre, p_string, strlen(p_string)+1);
}


CXMRand::CXMRand(void)
{
    srand(unsigned(time(0)));
}

uint32_t CXMRand::rand_(uint32_t start, uint32_t end)
{
    int rand_data;
    if((end-start)>RAND_MAX)
    {
        rand_data=(uint32_t)((uint32_t)(rand()*((uint32_t)RAND_MAX+1))+rand());
    }
    else
    {
        rand_data=rand();
    }
    return start+rand_data%(end-start+1);
}

double getDispersion(double *p_input_data, uint32_t input_data_num)
{
    if(p_input_data==NULL)
    {
        return 0;
    }
    if(input_data_num<=0)
    {
        return 0;
    }
    double temp_data=0, avg_data, dispersion_data=0;
    for(int iloop=0;iloop<input_data_num;iloop++)
    {
        temp_data+=p_input_data[iloop];
    }
    avg_data=temp_data/input_data_num;

    double temp_d;
    for(uint32_t iloop=0;iloop<input_data_num;iloop++)
    {
        temp_d = p_input_data[iloop] - avg_data;
        if (temp_d > 0)
        {
            dispersion_data += temp_d;
        }
        else
        {
            dispersion_data -= temp_d;
        }
    }
    return (dispersion_data/input_data_num/avg_data);
}

double getAverage(double *p_input_data, uint32_t input_data_num)
{
    if(p_input_data==NULL)
    {
        return 0;
    }
    if(input_data_num<=0)
    {
        return 0;
    }
    double temp_data=0, avg_data;
    for(uint32_t iloop=0;iloop<input_data_num;iloop++)
    {
        temp_data+=p_input_data[iloop];
    }
    avg_data=temp_data/input_data_num;
    return avg_data;
}


