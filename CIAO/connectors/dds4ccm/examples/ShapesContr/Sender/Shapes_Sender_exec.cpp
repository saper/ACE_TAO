// -*- C++ -*-
/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.0.0
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

#include "Shapes_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Shapes_Sender_Impl
{
  shape_changes_i::shape_changes_i (Sender_exec_i& callback)
     : callback_ (callback)
  {
  }

  void
  shape_changes_i::pass_change_event()
  {
    //there is a change in shape attributes, so get the new atrributes
     this->callback_.get_shape_attributes_get();
  }

  /**
   * Write action generator
   */
  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->pulse_callback_.tick ();
    return 0;
  }


  /**
   * Component Executor Implementation Class: Sender_exec_i
   */
  Sender_exec_i::Sender_exec_i (void)
    : rate_ (0)
      , max_x_ (100)
      , max_y_ (100)
      , max_size_ (25)
      , resize_shape_ (false)
      , color_ ("CYAN")
      , x_increasing_ (false)
      , y_increasing_ (false)
      , size_increasing_ (false)
  {
    ACE_NEW_THROW_EX (this->ticker_,
                      pulse_Generator (*this),
                      ::CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (this->shape_,
                      ShapeType,
                      ::CORBA::NO_MEMORY ());
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->ticker_;
    delete this->shape_;
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
    if (this->x_increasing_)
      {
        ++this->shape_->x;
        this->x_increasing_ = this->shape_->x + 1 <= this->max_x_;
      }
    else
      {
        --this->shape_->x;
        this->x_increasing_ = this->shape_->x - 1 < 0;
      }
    if (this->y_increasing_)
      {
        ++this->shape_->y;
        this->y_increasing_ = this->shape_->y + 1 <= this->max_y_;
      }
    else
      {
        --this->shape_->y;
        this->y_increasing_ = this->shape_->y - 1 < 0;
      }

    if (this->resize_shape_)
      {
        if (this->size_increasing_)
          {
            ++this->shape_->shapesize;
            this->size_increasing_ = this->shape_->shapesize + 1 <= this->max_size_;
          }
        else
          {
            --this->shape_->shapesize;
            this->size_increasing_ = this->shape_->shapesize - 1 < 0;
          }
      }

    try
    {
      Shapes::Writer_var writer =
        this->ciao_context_->get_connection_info_write_data ();
      if (! ::CORBA::is_nil (writer.in ()))
        {
          ::CORBA::Object_var cmp = writer->_get_component ();
          if (::CORBA::is_nil (cmp.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::tick - "
                                    "Unable to get component interface\n"));
              throw ::CORBA::INTERNAL ();
            }
          ::Shapes::CCM_DDS_Event_var conn =
            ::Shapes::CCM_DDS_Event::_narrow (cmp.in ());
          if (::CORBA::is_nil (conn.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::tick - "
                                    "Unable to narrow connector interface\n"));
              throw ::CORBA::INTERNAL ();
            }
          CORBA::String_var topic = conn->topic_name ();

          writer->write_one (*this->shape_, this->instance_handle_);
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("UPDATED Shape_info <%C> for <%C> %u:%u:%u\n"),
                    topic.in (),
                    this->shape_->color.in (),
                    this->shape_->x,
                    this->shape_->y,
                    this->shape_->shapesize));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "Sender_exec_i::tick - "
                    "ERROR: Writer seems nil.\n"));
        }
    }
    catch (const CCM_DDS::NonExistent& )
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Shape_info for <%C> not updated: didn't exist.\n"),
                  this->shape_->color.in ()));
    }
    catch (const CCM_DDS::InternalError& )
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Internal Error while updating Shape_info for <%C>.\n"),
                  this->shape_->color.in ()));
    }
  }

  void
  Sender_exec_i::start (void)
  {

    // calculate the interval time
    long const usec = 1000000 / this->rate_;
    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::start : Timer started.\n")));

  }
  void
  Sender_exec_i::register_topic()
  {
  // Register shape with dds.
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("REGISTER Shape_info for <%C> %u:%u:%u\n"),
                  this->shape_->color.in (),
                  this->shape_->x,
                  this->shape_->y,
                  this->shape_->shapesize));
    try
      {
        Shapes::Writer_var writer =
          this->ciao_context_->get_connection_info_write_data ();
        if (! ::CORBA::is_nil (writer.in ()))
          {
            this->instance_handle_ =
              writer->register_instance (*this->shape_);
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Sender_exec_i::ccm_activate - "
                      "ERROR: Writer seems nil.\n"));
          }
      }
    catch (const CCM_DDS::AlreadyCreated& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shape_info for <%C> already created.\n"),
                    this->shape_->color.in ()));
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating Shape_info for <%C>.\n"),
                      this->shape_->color.in ()));
      }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));

  }

  void
  Sender_exec_i::unregister_topic()
  {
    try
    {
      Shapes::Writer_var writer =
        this->ciao_context_->get_connection_info_write_data ();
      if (! ::CORBA::is_nil (writer.in ()))
        {
          writer->unregister_instance (*this->shape_,
                                       this->instance_handle_);
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "Sender_exec_i::stop - "
                    "ERROR: Writer seems nil.\n"));
        }
    }
    catch (const CCM_DDS::NonExistent& )
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Shape_info for <%C> not deleted: <%C> didn't exist.\n"),
                  this->shape_->color.in (), this->shape_->color.in ()));
    }
    catch (const CCM_DDS::InternalError& )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while deleting Shape_info for <%C>.\n"),
                  this->shape_->color.in ()));
    }

  }

  void
  Sender_exec_i::get_shape_attributes_get()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::get_shape_attributes\n")));
    shape_attributes_get_var shape_attrib_ = this->ciao_context_->get_connection_shapeAttribsSub ();
    if (CORBA::is_nil (shape_attrib_.in ()))
      throw CORBA::BAD_INV_ORDER ();

    //CORBA::String_var color_ = shape_attrib_->color ();
    CORBA::UShort rate = shape_attrib_->publish_rate();
    this->max_size_ = shape_attrib_->max_size();
    this->max_x_ = shape_attrib_->max_x();
    this->max_y_ = shape_attrib_->max_y();
    this->resize_shape_ = shape_attrib_->resize();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::get_shape_attributes ")
                          ACE_TEXT (" color = %C, rate= %d, resize = %d, max_size = %d, max_x = %d, max_y = %d, resize = %d \n"),
                          color_.in(), rate, this->resize_shape_, this->max_size_,
                          this->max_x_, this->max_y_, this->resize_shape_));


    this->shape_->color = CORBA::string_dup(color_);

    if (rate != this->rate_)
    {
      //rate of publishing is changed so stop and start timer.
      this->stop();
      this->rate_ = rate;
      if (this->rate_ != 0)
      {
        this->start();
      }
    }

  }

  // Component attributes and port operations.
  char*
  Sender_exec_i::color (void)
  {
    return ::CORBA::string_dup (this->color_);
  }

  void
  Sender_exec_i::color (const char* color)
  {
    this->color_ = ::CORBA::string_dup (color);
  }

  CCM_shape_changes*
  Sender_exec_i::get_shapeChangesPub ()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::get_shapeChangesPub\n")));
    return ( new shape_changes_i (*this) );
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::set_session_context\n")));
    this->ciao_context_ =
      ::Shapes::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::configuration_complete\n")));
    // Set the initial square properties right now
    this->shape_->x = ACE_OS::rand () % this->max_x_;
    this->shape_->y = ACE_OS::rand () % this->max_y_;
    this->shape_->shapesize = this->max_size_;
    this->shape_->color = CORBA::string_dup(this->color_);
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::ccm_activate\n")));
    this->register_topic();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
    this->unregister_topic();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Shapes_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
