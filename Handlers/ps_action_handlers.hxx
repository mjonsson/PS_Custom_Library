/*!
 * \file ps_action_handlers.hxx
 * \date 2014/04/30 22:45
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Teamcenter action handlers.
 *
 * \warning After installing handlers make sure to add the line "<error id="100">%1$</error>" to the "TC_ROOT\lang\textserver\en_US\ue_errors.xml" file.
 *
*/

#pragma once

#include "ps_global.hxx"

//! Creates a dispatcher request based on input parameters.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_create_dispatcher_request_ah(EPM_action_message_t msg);

//! Copies a property from EPMTask in context to an object attached as target to root task.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_copy_task_properties_ah(EPM_action_message_t msg);

//! Moves attachments from target to reference or vice versa.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_move_attachments_ah(EPM_action_message_t msg);

//! Starts a timer with identity.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_timer_start_ah(EPM_action_message_t msg);

//! Stops a timer with identity.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_timer_stop_ah(EPM_action_message_t msg);
