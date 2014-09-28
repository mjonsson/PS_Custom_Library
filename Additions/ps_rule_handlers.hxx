/*!
 * \file ps_rule_handlers.hxx
 * \date 2014/04/30 22:45
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Teamcenter rule handlers
 *
 * \warning After installing handlers make sure to add the line "<error id="100">%1$</error>" to the "TC_ROOT\lang\textserver\en_US\ue_errors.xml" file.
 *
*/

#pragma once

#include "ps_global.hxx"

//! Checks that an object assigned as target to a root task is not already withing another process.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_in_process_rh(EPM_rule_message_t msg);

//! Checks that the initiator of a process fulfills certain pre-requisities.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_initiator_rh(EPM_rule_message_t msg);

//! Checks that the initiator of a process have certain access to all objects attached as target to the root task.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_privileges_rh(EPM_rule_message_t msg);

//! Checks that mandatory properties are set correctly.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_properties_rh(EPM_rule_message_t msg);

//! Verifies the structure maturity of a target attached to the root task.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_child_structure_rh(EPM_rule_message_t msg);

//! Verify that target does not have an invalid structure parent.
/*!
 *  \note See exernal documentation for handler usage in Teamcenter.
 */
int ps_check_where_used_rh(EPM_rule_message_t msg);