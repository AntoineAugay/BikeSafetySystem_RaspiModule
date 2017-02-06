
#include "String_queue.h"

StringQueue::StringQueue(){
    begin = nullptr;
    end = nullptr;
}


void StringQueue::push(std::string str){

    /* Creation of a new element */
    element *new_element = new element;

    /* Update element */
    new_element->str		= str;
    new_element->next		= begin;
    new_element->previous	= nullptr;
    
    /* Update class pointer */
    if(begin != nullptr){
        begin->previous = new_element;
    } else {
        end = new_element;
    }
    
    begin = new_element;
}

std::string StringQueue::pop(){

    /* Return empty string if the list is empty */
    if(end == nullptr){
        return "";
    }
    
    /* Get last element */    
    element *last_element = end;
  
    /* Update class pointer */
    end = last_element->previous;
    
    if(end == nullptr){
      begin = end;
    } else {
      end->next = nullptr;
    }
    
    /* Get the string */
	std::string str_out = last_element->str;


    /* Delete the element*/
    delete last_element;
    
    /* return the String */
    return str_out;
 
}

