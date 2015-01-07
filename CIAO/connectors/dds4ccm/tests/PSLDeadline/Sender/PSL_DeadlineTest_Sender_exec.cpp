// -*- C++ -*-
/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v1.8.3
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.dre.vanderbilt.edu/~schmidt/TAO.html
 **/

#include "PSL_DeadlineTest_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_PSL_DeadlineTest_Sender_Impl
{

  /**
   * Write action generator
   */

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->pulse_callback_.tick ();
    return 0;
  }

  /**
   * Facet Executor Implementation Class: test_topic_connector_status_exec_i
   */

  test_topic_connector_status_exec_i::test_topic_connector_status_exec_i (
        ::PSL_DeadlineTest::CCM_Sender_Context_ptr ctx)
    : ciao_context_ (
        ::PSL_DeadlineTest::CCM_Sender_Context::_duplicate (ctx))
  {
  }

  test_topic_connector_status_exec_i::~test_topic_connector_status_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener

  void
  test_topic_connector_status_exec_i::on_inconsistent_topic (::DDS::Topic_ptr /* the_topic */,
  const ::DDS::InconsistentTopicStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  test_topic_connector_status_exec_i::on_requested_incompatible_qos (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::RequestedIncompatibleQosStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  test_topic_connector_status_exec_i::on_sample_rejected (::DDS::DataReader_ptr /* the_reader */,
  const ::DDS::SampleRejectedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  test_topic_connector_status_exec_i::on_offered_deadline_missed (::DDS::DataWriter_ptr /* the_writer */,
  const ::DDS::OfferedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  test_topic_connector_status_exec_i::on_offered_incompatible_qos (::DDS::DataWriter_ptr /* the_writer */,
  const ::DDS::OfferedIncompatibleQosStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  test_topic_connector_status_exec_i::on_unexpected_status (::DDS::Entity_ptr /* the_entity */,
  ::DDS::StatusKind /* status_kind */)
  {
    /* Your code here. */
  }

  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  Sender_exec_i::Sender_exec_i (void)
  {
    ACE_NEW_THROW_EX (this->ticker_,
                      pulse_Generator (*this),
                      ::CORBA::NO_MEMORY ());
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.
  ACE_Reactor*
  Sender_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  void
  Sender_exec_i::tick ()
  {
    ::PSL_DeadlineTest::PSLDeadlineConnector::Writer_var writer =
      this->ciao_context_->get_connection_test_topic_write_data ();

    for (PSL_DLTest_Table::iterator i = this->_ktests_.begin ();
         i != this->_ktests_.end ();
         ++i)
      {
         try
          {
            if (! ::CORBA::is_nil (writer.in ()))
              {
                writer->write_one(i->second,::DDS::HANDLE_NIL);
                i->second->x++;
              }
          }
         catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating topic for <%C>.\n"),
                    i->first.c_str ()));
          }
       }
  }

  void
  Sender_exec_i::start (void)
  {
    // deadline period in QoS is set to two so the reader expects data every
    // two seconds. Setting the interval to somewhat more than two seconds,
    // the reader should (almost) always receive an on_requested_deadline_missed
    // callback from DDS.
    long sec = 2L;
    long const usec = 100000L;
    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (sec, usec),
                ACE_Time_Value (sec, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
      }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
  }

  void
  Sender_exec_i::add_instance_of_topic (const char * key, int x)
  {
    TestTopic *new_key = new TestTopic;
    new_key->key = CORBA::string_dup(key);
    new_key->x = x;
    this->_ktests_[key] = new_key;
  }

  // Component attributes and port operations.

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_test_topic_connector_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_test_topic_connector_status_.in ()))
      {
        test_topic_connector_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          test_topic_connector_status_exec_i (
            this->ciao_context_.in ()),
            ::CCM_DDS::CCM_ConnectorStatusListener::_nil ());

          this->ciao_test_topic_connector_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_ConnectorStatusListener::_duplicate (
        this->ciao_test_topic_connector_status_.in ());
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::PSL_DeadlineTest::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    //add 2 different instances of topic
    this->add_instance_of_topic ("ONE", 1);
    this->add_instance_of_topic ("ONE", 2);
    this->add_instance_of_topic ("ONE", 3);
    this->add_instance_of_topic ("ONE", 4);
    this->add_instance_of_topic ("ONE", 5);
    this->add_instance_of_topic ("TWO", 1);
    this->add_instance_of_topic ("TWO", 2);
    this->add_instance_of_topic ("TWO", 3);
    this->add_instance_of_topic ("TWO", 4);
    this->add_instance_of_topic ("TWO", 5);
    this->start ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_DeadlineTest_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
