#ifndef STRING_QUEUE_H
#define STRING_QUEUE_H


/*!
 * \file String_queue.h
 * \brief String queue
 * \author AUGAY Antoine
 * \version 0.1
 */

#include <string>

struct element {
    std::string str;
    element *next;
    element *previous;
};


/*! \class String_queue
   * \brief
   *
   */
class StringQueue {
private:

    element *begin; /*!< pointer on the first element */
    element *end;   /*!< pointer on the last element */


public:


    /*!
     *  \brief Constructeur
     *
     *  \param nothing
     */
    StringQueue();

    /*!
     *  \brief add the message at the begin of the list
     *
     *  \param str, String to add
     *  \return nothing
     */
    void push(std::string str);

    /*!
     *  \brief get the oldest message in the list
     *
     *  \param nothing
     *  \return the last message added in the list
     */
    std::string pop();

};


#endif //STRING_QUEUE_H
