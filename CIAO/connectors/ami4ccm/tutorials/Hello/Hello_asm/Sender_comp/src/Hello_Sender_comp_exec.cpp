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

#include "Hello_Sender_comp_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Sender_comp_Impl
{

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
       ::Hello::CCM_Sender_comp_Context_ptr context)
      : context_(::Hello::CCM_Sender_comp_Context::_duplicate (context))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (2);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :")
                ACE_TEXT("\tInvoke Asynchronous calls\n")));
    ::Hello::AMI4CCM_MyFoo_obj_var my_foo_ami_ =
        this->context_->get_connection_sendc_run_my_foo();

    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("ERROR Sender (ASYNCH) :")
                      ACE_TEXT("\tfoo_ami is NIL !!!\n")));
        return 1;
      }
    ::Hello::CCM_AMI4CCM_MyFoo_objReplyHandler_var cb =
       new AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i ();

    for (int i = 0; i < 5; ++i)
      {
        my_foo_ami_->sendc_foo (cb.in (),
                                "Do something asynchronous");
        my_foo_ami_->sendc_hello (cb.in ());
        my_foo_ami_->sendc_get_rw_attrib(cb.in ());
        my_foo_ami_->sendc_set_rw_attrib(cb.in (),
                                          15);
        my_foo_ami_->sendc_get_ro_attrib(cb.in ());
        ACE_DEBUG ((LM_DEBUG,
           ACE_TEXT("Sender (ASYNCH) :\tInvoked 5 Asynchronous calls\n")));
      }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoke Asynchronous calls ")
                ACE_TEXT("to test except handling\n")));
    my_foo_ami_->sendc_foo (cb.in (), "");
    my_foo_ami_->sendc_set_rw_attrib(cb.in (), 0);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (ASYNCH) :\tInvoked 2 Asynchronous call.\n")));
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
      ::Hello::CCM_Sender_comp_Context_ptr context)
  : context_(::Hello::CCM_Sender_comp_Context::_duplicate (context))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (2);
    ::Hello::MyFoo_obj_var my_foo_ami_ =
        this->context_->get_connection_run_my_foo ();
    // run synch calls
    CORBA::String_var out_str;
    for (int i = 0; i < 2; ++i)
      {
        try
          {
            ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT("Sender (SYNCH):\tInvoke synchronous call (foo)\n")));
            CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                   out_str.out());
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call (foo)\n")
                    ACE_TEXT(" result <%u> answer <%C>\n"),
                             result, out_str.in()));

            CORBA::Long answer;
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoke synchronous call")
                    ACE_TEXT(" (hello)\n")));
            my_foo_ami_->hello (answer);
            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoked synchronous ")
                    ACE_TEXT("call  (hello) result <%u>\n"), answer));

            ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("Sender (SYNCH):\tInvoke synchronous call ")
                    ACE_TEXT("(rw_attrib())\n")));
            CORBA::Short rw_attrib = my_foo_ami_->rw_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                ACE_TEXT("(rw_attrib()) return <%u>\n"), rw_attrib));

            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoke synchronous call")
                ACE_TEXT(" (rw_attrib(15))\n")));
            my_foo_ami_->rw_attrib (15);
            ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                 ACE_TEXT("(rw_attrib(15))\n")));

            ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (SYNCH):\tInvoke synchronous call")
                ACE_TEXT(" (ro_attrib())\n")));
            CORBA::Short ro_attrib = my_foo_ami_->ro_attrib ();
            ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("Sender (SYNCH):\tInvoked synchronous call ")
                 ACE_TEXT("(ro_attrib) return <%u>\n"), ro_attrib));
          }
        catch (const Hello::InternalError& ex)
          {
            ACE_ERROR ((LM_ERROR,
                ACE_TEXT("Sender (SYNCH) :\tERROR: ")
                ACE_TEXT("Unexpected Exception caught :")
                ACE_TEXT(" <%u> <%C>\n"), ex.id, ex.error_string.in ()));
           }
      }
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_comp_exec_i
  //============================================================
  Sender_comp_exec_i::Sender_comp_exec_i (void) :
    asynch_foo_gen (0),
    synch_foo_gen (0)
  {
  }

  Sender_comp_exec_i::~Sender_comp_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  // Operations from Components::SessionComponent.

  void
  Sender_comp_exec_i::set_session_context (
     ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
       ::Hello::CCM_Sender_comp_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_comp_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sender_comp_exec_i::ccm_activate (void)
  {
    this->asynch_foo_gen =
          new asynch_foo_generator (this->ciao_context_.in ());
    this->asynch_foo_gen ->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    this->synch_foo_gen =
       new synch_foo_generator (this->ciao_context_.in ());
    this->synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_comp_exec_i::ccm_passivate (void)
  {
     /* Your code here. */
  }

  void
  Sender_comp_exec_i::ccm_remove (void)
  {
    delete this->asynch_foo_gen;
    this->asynch_foo_gen = 0;
    delete this->synch_foo_gen;
    this->synch_foo_gen = 0;
  }

  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i (void)
  {
  }

  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::~AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i (void)
  {
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::foo (
      ::CORBA::Long ami_return_val,
      const char * answer)
  {
      /* Your code here. */
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Sender:\tMyFoo AMI Callback from foo:")
                  ACE_TEXT("result <%u> answer <%C>\n"),
                  ami_return_val, answer));
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::foo_excep (
    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const Hello::InternalError& ex)
      {
         ACE_DEBUG ((LM_DEBUG,
                   ACE_TEXT("Sender:\tMyFoo Expected excception AMI Callback ")
                   ACE_TEXT("from foo: id <%u> error_string <%C>\n"),
                   ex.id, ex.error_string.in ()));
      }
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::hello (
    ::CORBA::Long answer)
  {
    ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT("Sender:\tMyFoo AMI Callback from hello:")
                 ACE_TEXT(" answer <%u>\n"), answer));
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::hello_excep (
    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    try
       {
         excep_holder->raise_exception ();
       }
     catch (const Hello::InternalError& ex)
       {
         ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT("Sender:\tMyFoo Exception AMI Callback from hello:\n")
               ACE_TEXT(" id <%u> error_string <%C>\n"),
               ex.id, ex.error_string.in ()));
       }
   }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::get_rw_attrib (
    ::CORBA::Short rw_attrib)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from get_rw_attrib:")
                ACE_TEXT(" result <%u>\n"), rw_attrib));
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::set_rw_attrib (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from ")
                ACE_TEXT("set_rw_attrib.\n")));
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
   }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::get_ro_attrib (
    ::CORBA::Short ro_attrib)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender:\tMyFoo AMI Callback from ")
                ACE_TEXT("get_ro_attrib: result <%u>\n"), ro_attrib));
  }

  void
  AMI4CCM_MyFoo_objReplyHandler_run_my_foo_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder_ptr  excep_holder)
  {
    excep_holder->raise_exception ();
  }

  extern "C" HELLO_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_comp_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_comp_exec_i);

    return retval;
  }
}
