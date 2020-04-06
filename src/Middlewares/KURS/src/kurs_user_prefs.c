/*
 * user_settings.c
 *
 *  Created on: Nov 6, 2017
 *      Author: DC
 */
#include <stddef.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <inttypes.h>
#include "../inc/kurs_user_prefs.h"
//#include "../../../configs.h"

#ifndef UNUSED
#define UNUSED(_param) _param = _param;
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif
char save = 0;
#ifndef USER_PREFS_MAX_COUNT
#define USER_PREFS_MAX_COUNT		10240								//!<maximum size in bytes of the user preferences.
#endif
#ifndef USER_PREFS_SIGNATURE
#define USER_PREFS_SIGNATURE		"65e4eef092ce4ca6a4678a7f5bb0532b"
//!<unique signature used to validate the user preferences (generated through https://www.uuidgenerator.net/)
#define USER_PREFS_SIGNATURE_LEN	32									//!<length of the user preferences unique signature
#endif
#ifndef PAGE_SIZE
#define PAGE_SIZE 256
#endif
char* usBuffer;

struct preference_node_t{
	size_t size;
	void*  value;
	struct preference_node_t *next;
};

typedef struct preference_node_t node;

struct list{
	int count; 	/*preferences items count*/
	int size; 	/*size in bytes*/
	node *head;
	node *last;
};
struct list my_list;
/**
 * Gets the number of preferences in the list
 * @return
 */
uint32_t get_preferences_list_count(){
	return my_list.count;
}

uint32_t get_preferences_list_size(){
	return my_list.size;
}

int append_preference(void* data, size_t size){
    node *temp_node;
    temp_node = (node *) calloc(1,sizeof(node));

    temp_node->value = data;
    temp_node->size = size;
    temp_node->next = NULL;

    //For the 1st element
    if(my_list.head==NULL){
    	my_list.head=temp_node;
    	my_list.last=temp_node;
    }else{
    	my_list.last->next=temp_node;
    	my_list.last=temp_node;
    }
    my_list.count++;
    my_list.size += size;
    return size;
}

ret_code_t list_to_array(char* _array){
	if(_array == NULL){
		return RET_FAIL;
	}
	int ptr = 0;
	node* n = my_list.head;
    while(n != NULL){
    	memcpy(&_array[ptr], n->value, n->size);
    	ptr += n->size;
        n = n->next;
    }
    return RET_SUCCESS;
}

ret_code_t array_to_list(char* _array){
	if(_array == NULL){
		return RET_FAIL;
	}
	int ptr = 0;
	node* n = my_list.head;
    while(n != NULL){
    	memcpy(n->value, &_array[ptr], n->size);
    	ptr += n->size;
        n = n->next;
    }
    return RET_SUCCESS;
}

/**Function that writes the user preferences bytes to flash.
 * It also calculates the CRC, appends to the data as well as the UUID signature.
 * @param data	pointer to the user preferences
 * @param count	number of data bytes to write
 * @since 0.0.1
 */
void commit_user_settings(){
	int offset = 0;
	size_t size = get_preferences_list_size();
	int total_size = size+sizeof(int)+2*USER_PREFS_SIGNATURE_LEN;
//	int align = PAGE_SIZE - (total_size%PAGE_SIZE);
//	total_size += align;
	usBuffer = (char*) malloc(total_size);

	// first insert signature
	memcpy(&usBuffer[offset], USER_PREFS_SIGNATURE, USER_PREFS_SIGNATURE_LEN);
	offset += USER_PREFS_SIGNATURE_LEN;
	// then insert actual length of user settings (in bytes)
	memcpy(&usBuffer[offset], &size, sizeof(int));
	offset += sizeof(int);
	// then copy user settings
	if(list_to_array(&usBuffer[offset]) != RET_SUCCESS){
		free(usBuffer);
		return;
	}
	offset += size;
	// finally append signature again
	memcpy(&usBuffer[offset], USER_PREFS_SIGNATURE, USER_PREFS_SIGNATURE_LEN);
	offset += USER_PREFS_SIGNATURE_LEN;

	IPGLB_WriteUserPrefsCallback(usBuffer, total_size);
	free(usBuffer);
}

/**Reads user preferences data from flash.
 * It also validates the data by checking for CRC and signature.
 * @param data	pointer to where the read data should be stored
 * @param count	number of data bytes to read.
 * @return \c RET_SUCCESS on success.
 * @since 0.0.1
 */
ret_code_t read_user_settings(void){
	ret_code_t rc = RET_FAIL;
	size_t size = get_preferences_list_size();
	int total_size = size+sizeof(int)+2*USER_PREFS_SIGNATURE_LEN;
//	int align = PAGE_SIZE - (total_size%PAGE_SIZE);
//	total_size += align;
	usBuffer = (char*) malloc(total_size);

	rc = IPGLB_ReadUserPrefsCallback(usBuffer, total_size);
	if(rc != RET_SUCCESS){
		free(usBuffer);
		// hardware failed before we were able to read the whole data
		return rc;
	}
	// check data signature at beginning
	if(memcmp(&usBuffer[0], USER_PREFS_SIGNATURE, USER_PREFS_SIGNATURE_LEN) != 0){
		free(usBuffer);
		rc = RET_FAIL;
		return rc;// stored signature does not match
	}
	// Compare count
	int detected_count = 0;
	memcpy(&detected_count, &usBuffer[USER_PREFS_SIGNATURE_LEN], sizeof(int));
	if(detected_count != size){
		free(usBuffer);
		rc = RET_FAIL;
		return rc;
	}
	// check data signature at end
	if(memcmp(&usBuffer[USER_PREFS_SIGNATURE_LEN+size+sizeof(int)], USER_PREFS_SIGNATURE, USER_PREFS_SIGNATURE_LEN) != 0){
		free(usBuffer);
		rc = RET_FAIL;
		return rc;// stored signature does not match
	}
	// at this point, all checks have passed
	rc = array_to_list(&usBuffer[USER_PREFS_SIGNATURE_LEN + sizeof(int)]);
	free(usBuffer);
	return RET_SUCCESS;
}

__weak void IPGLB_WriteUserPrefsCallback(void * pSrc, size_t dwSize){
	UNUSED(pSrc);
	UNUSED(dwSize);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
}

__weak ret_code_t IPGLB_ReadUserPrefsCallback(void* pDst, size_t dwSize){
	UNUSED(pDst);
	UNUSED(dwSize);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
	return RET_FAIL;
}

__weak ret_code_t IPGLB_ReadLngFileCallback(void* pDst, size_t dwSize){
	UNUSED(pDst);
	UNUSED(dwSize);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
	return RET_FAIL;
}

__weak void IPGLB_WriteLngFileCallback(void * pSrc, size_t dwSize){
	UNUSED(pSrc);
	UNUSED(dwSize);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
}
