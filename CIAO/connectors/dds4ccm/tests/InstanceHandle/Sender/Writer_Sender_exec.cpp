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

#include "Writer_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#include "Connector/Writer_Connector_conn.h"
#include "dds4ccm/impl/Utils.h"
#if (CIAO_DDS4CCM_NDDS == 1)
# include "dds4ccm/impl/ndds/DataWriter_T.h"
#endif

namespace CIAO_Writer_Sender_Impl
{
  /**
   * Starter
   */

  StartHandler::StartHandler(Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  StartHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.run ();
    return 0;
  }

  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  Sender_exec_i::Sender_exec_i (void)
    : keys_ (5),
      dds_writer_ (0)
  {
    ACE_NEW_THROW_EX (this->rh_,
                      StartHandler (*this),
                      ::CORBA::NO_MEMORY ());
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->rh_;
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
  Sender_exec_i::unregister_handles ()
  {
    ::WriterTestConnector::Writer_var ccm_writer =
      this->ciao_context_->get_connection_info_write_data ();

    for (Writer_Table::iterator i = this->ktests_.begin ();
         i != this->ktests_.end ();
         ++i)
      {
        try
          {
            ::DDS::InstanceHandle_t const hnd = this->handles_[i->first.c_str ()];
            ccm_writer->unregister_instance (i->second, hnd);
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Unregistered <%C> - valid ")
                        DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT ("\n"),
                        i->first.c_str (),
                        DDS_INSTANCE_HANDLE_LOG(hnd)));
          }
        catch (...)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                    ACE_TEXT ("unknown exception caught during unregister_instance.\n")));
          }
      }
  }

  void
  Sender_exec_i::register_handles ()
  {
    ::WriterTestConnector::Writer_var ccm_writer =
      this->ciao_context_->get_connection_info_write_data ();
    Writer_Table::iterator i = this->ktests_.begin ();
    for (i = this->ktests_.begin(); i != this->ktests_.end(); ++i)
      {
        DDS::InstanceHandle_t const hnd =
          ccm_writer->register_instance (i->second);
        if (hnd == ::DDS::HANDLE_NIL)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("ERROR: Unable to register handle for <%C>\n"),
                        i->first.c_str ()));
          }
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Registering instance for <%C>\n"),
                    i->second->key.in ()));
        this->handles_[i->first.c_str ()] = hnd;
     }
  }

  void
  Sender_exec_i::test_equality ()
  {
#if (CIAO_DDS4CCM_NDDS == 1)
    Writer_Table::iterator i = this->ktests_.begin ();
    for (i = this->ktests_.begin(); i != this->ktests_.end(); ++i)
      {
        ::DDS::InstanceHandle_t ccm_hnd = this->handles_[i->first.c_str ()];

        DDS_InstanceHandle_t dds_hnd =
          this->dds_writer_->lookup_instance (i->second);
        ::DDS::InstanceHandle_t ccm_dds_hnd;
        ccm_dds_hnd <<= dds_hnd;
        if (ccm_hnd == ccm_dds_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "== operator seems to work for CCM handles "
                          "for key <%C>\n",
                          i->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "== operator doesn't seem to work for CCM "
                          "handles for key <%C>\n",
                          i->first.c_str ()));
          }
      }
#endif
  }

  void
  Sender_exec_i::test_non_equality ()
  {
#if (CIAO_DDS4CCM_NDDS == 1)
    Writer_Table::iterator i = this->ktests_.begin ();
    for (i = this->ktests_.begin(); i != this->ktests_.end(); ++i)
      {
        ::DDS::InstanceHandle_t ccm_hnd = this->handles_[i->first.c_str ()];

        Writer_Table::iterator unequal = i;
        ++unequal;
        if (unequal == this->ktests_.end ())
          {
            unequal = this->ktests_.begin ();
          }

        DDS_InstanceHandle_t const dds_hnd =
          this->dds_writer_->lookup_instance (unequal->second);
        DDS::InstanceHandle_t ccm_dds_hnd;
        ccm_dds_hnd <<= dds_hnd;
        if (ccm_dds_hnd != ccm_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "!= operator seems to work for CCM "
                          "handles for key <%C>\n",
                          i->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "!= operator doesn't seem to work for CCM "
                          "handles for key <%C>\n",
                          i->first.c_str ()));
          }
      }
    unregister_handles ();
#endif
  }

  void
  Sender_exec_i::run ()
  {
    test_equality ();
    test_non_equality ();
  }

  void
  Sender_exec_i::start (void)
  {
    this->reactor ()->notify (this->rh_);
  }

  // Component attributes and port operations.

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (
    const ::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Writer::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
#if (CIAO_DDS4CCM_NDDS == 1)
    typedef ::CIAO::NDDS::DDS_DataWriter_Base DataWriter_type;

    ::DDS::DataWriter_var dds_dw =
      this->ciao_context_->get_connection_info_write_dds_entity ();

    ::CIAO::DDS4CCM::CCM_DataWriter * ccm_dw =
      dynamic_cast < ::CIAO::DDS4CCM::CCM_DataWriter * > (dds_dw.in ());

    if (ccm_dw)
      {

        ::DDS::DataWriter_var tmp = ccm_dw->get_dds_entity ();
        if (! ::CORBA::is_nil (tmp.in ()))
          {
            DataWriter_type * typed_ccm_dw = dynamic_cast <DataWriter_type *> (tmp.in ());
            if (typed_ccm_dw)
              {
                DDSDataWriter* dds_datawriter = typed_ccm_dw->get_rti_entity ();
                if (dds_datawriter)
                  {
                    this->dds_writer_ =
                      ::CIAO_WriterTestConnector_DDS_Event_Impl::WriterTest_DDS_Traits::datawriter_type::narrow (dds_datawriter);
                    if (!this->dds_writer_)
                      {
                        ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                                    "Error narrowing to a typed "
                                    "DDS DataWriter.\n"));
                        throw ::CORBA::INTERNAL ();
                      }
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                                "Error getting DDS DataWriter.\n"));
                    throw ::CORBA::INTERNAL ();
                  }
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                            "Error casting DataWriter to typed DataWriter\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                    "::DDS::DataWriter of CCM DataWriter seems nil\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::configuration_complete - "
                    "Error casting DataWriter to CCM DataWriter\n"));
        throw ::CORBA::INTERNAL ();
      }
#endif
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->start ();

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        WriterTest *new_key = 0;
        ACE_NEW_THROW_EX (new_key,
                          WriterTest,
                          ::CORBA::NO_MEMORY ());
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);

        this->ktests_[key] = new_key;
      }
    register_handles ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Writer_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
