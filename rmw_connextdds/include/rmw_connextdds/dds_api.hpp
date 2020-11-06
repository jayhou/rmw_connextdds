/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef RMW_CONNEXTDDS__DDS_API_HPP_
#define RMW_CONNEXTDDS__DDS_API_HPP_

#include "rmw_connextdds/static_config.hpp"


#if RMW_CONNEXT_DDS_API == RMW_CONNEXT_DDS_API_MICRO
#include "rti_me_c.h"
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"
#include "netio/netio_udp.h"
#include "netio_shmem/netio_shmem.h"
// #include "sec_core/sec_core_c.h"
#include "rtiros/rtime_ext.h"
#include "REDASequence.h"
#elif RMW_CONNEXT_DDS_API == RMW_CONNEXT_DDS_API_PRO
#include "ndds/ndds_c.h"
#else
#error "invalid DDS API selected"
#endif /* RMW_CONNEXT_DDS_API */

#ifndef UNUSED_ARG
#define UNUSED_ARG(arg_)        (void)(arg_)
#endif /* UNUSED_ARG */

#include "rmw_connextdds/type_support_impl.hpp"

#include "rmw/rmw.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"

class RMW_Connext_MessageTypeSupport;
class RMW_Connext_Publisher;
class RMW_Connext_Subscriber;
class RMW_Connext_StdSubscriberStatusCondition;
struct RMW_Connext_Message;

enum RMW_Connext_MessageType
{
    RMW_CONNEXT_MESSAGE_USERDATA,
    RMW_CONNEXT_MESSAGE_REQUEST,
    RMW_CONNEXT_MESSAGE_REPLY
};

extern const char * const RMW_CONNEXTDDS_ID;
extern const char * const RMW_CONNEXTDDS_SERIALIZATION_FORMAT;

rmw_ret_t
rmw_connextdds_set_log_verbosity(rmw_log_severity_t severity);

rmw_ret_t
rmw_connextdds_initialize_participant_factory(
    rmw_context_impl_t *const ctx);

rmw_ret_t
rmw_connextdds_finalize_participant_factory(
    rmw_context_impl_t *const ctx);

rmw_ret_t
rmw_connextdds_initialize_participant_qos_impl(
    rmw_context_impl_t *const ctx,
    DDS_DomainParticipantQos *const dp_qos);

rmw_ret_t
rmw_connextdds_get_datawriter_qos(
    rmw_context_impl_t *const ctx,
    RMW_Connext_MessageTypeSupport *const type_support,
    DDS_DataWriterQos *const qos,
    const rmw_qos_profile_t *const qos_policies
#if RMW_CONNEXT_HAVE_OPTIONS
    ,
    const rmw_publisher_options_t *const pub_options
#endif /* RMW_CONNEXT_HAVE_OPTIONS */
    );

rmw_ret_t
rmw_connextdds_get_datareader_qos(
    rmw_context_impl_t *const ctx,
    RMW_Connext_MessageTypeSupport *const type_support,
    DDS_DataReaderQos *const qos,
    const rmw_qos_profile_t *const qos_policies
#if RMW_CONNEXT_HAVE_OPTIONS
    ,
    const rmw_subscription_options_t *const sub_options
#endif /* RMW_CONNEXT_HAVE_OPTIONS */
    );

DDS_DataWriter*
rmw_connextdds_create_datawriter(
    rmw_context_impl_t *const ctx,
    DDS_DomainParticipant *const participant,
    DDS_Publisher *const pub,
    const rmw_qos_profile_t *const qos_policies,
#if RMW_CONNEXT_HAVE_OPTIONS
    const rmw_publisher_options_t *const publisher_options,
#endif /* RMW_CONNEXT_HAVE_OPTIONS */
    const bool internal,
    RMW_Connext_MessageTypeSupport *const type_support,
    DDS_Topic *const topic,
    DDS_DataWriterQos *const dw_qos);

DDS_DataReader*
rmw_connextdds_create_datareader(
    rmw_context_impl_t *const ctx,
    DDS_DomainParticipant *const participant,
    DDS_Subscriber *const sub,
    const rmw_qos_profile_t *const qos_policies,
#if RMW_CONNEXT_HAVE_OPTIONS
    const rmw_subscription_options_t *const subscriber_options,
#endif /* RMW_CONNEXT_HAVE_OPTIONS */
    const bool internal,
    RMW_Connext_MessageTypeSupport *const type_support,
    DDS_Topic *const topic,
    DDS_DataReaderQos *const dr_qos);

rmw_ret_t
rmw_connextdds_write_message(
    RMW_Connext_Publisher *const pub,
    RMW_Connext_Message *const message,
    int64_t *const sn_out);

rmw_ret_t
rmw_connextdds_take_samples(
    RMW_Connext_Subscriber *const sub);

rmw_ret_t
rmw_connextdds_return_samples(
    RMW_Connext_Subscriber *const sub);

rmw_ret_t
rmw_connextdds_filter_sample(
    RMW_Connext_Subscriber *const sub,
    const void *const sample,
    const DDS_SampleInfo *const info,
    bool *const accepted);

RMW_Connext_MessageTypeSupport*
rmw_connextdds_register_type_support(
    rmw_context_impl_t *const ctx,
    const rosidl_message_type_support_t *const type_supports,
    DDS_DomainParticipant *const participant,
    bool& registered,
    const RMW_Connext_MessageType message_type,
    const void *const intro_members,
    const bool intro_members_cpp,
    const char *const type_name);

rmw_ret_t
rmw_connextdds_unregister_type_support(
    rmw_context_impl_t *const ctx,
    DDS_DomainParticipant *const participant,
    const char *const type_name);

rmw_ret_t
rmw_connextdds_dcps_participant_get_reader(
    rmw_context_impl_t *const ctx,
    DDS_DataReader **const reader_out);

rmw_ret_t
rmw_connextdds_dcps_publication_get_reader(
    rmw_context_impl_t *const ctx,
    DDS_DataReader **const reader_out);

rmw_ret_t
rmw_connextdds_dcps_subscription_get_reader(
    rmw_context_impl_t *const ctx,
    DDS_DataReader **const reader_out);

rmw_ret_t
rmw_connextdds_enable_builtin_readers(rmw_context_impl_t *const ctx);

rmw_ret_t
rmw_connextdds_dcps_participant_on_data(rmw_context_impl_t *const ctx);

rmw_ret_t
rmw_connextdds_dcps_publication_on_data(rmw_context_impl_t *const ctx);

rmw_ret_t
rmw_connextdds_dcps_subscription_on_data(rmw_context_impl_t *const ctx);

void
rmw_connextdds_ih_to_gid(
    const DDS_InstanceHandle_t &ih, rmw_gid_t &gid);

void
rmw_connextdds_configure_subscriber_condition_listener(
    RMW_Connext_Subscriber *const sub,
    RMW_Connext_StdSubscriberStatusCondition *cond,
    DDS_DataReaderListener *const listener,
    DDS_StatusMask *const listener_mask);

void
rmw_connextdds_builtinkey_to_guid(
    const DDS_BuiltinTopicKey_t *const self,
    DDS_GUID_t *const dst);

#endif  // RMW_CONNEXTDDS__DDS_API_HPP_
