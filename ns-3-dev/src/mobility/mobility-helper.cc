#include "mobility-helper.h"
#include "mobility-model.h"
#include "mobility-model-notifier.h"
#include "position-allocator.h"

namespace ns3 {

MobilityHelper::MobilityHelper ()
  : m_notifierEnabled (false)
{}
void 
MobilityHelper::EnableNotifier (void)
{
  m_notifierEnabled = true;
}
void 
MobilityHelper::DisableNotifier (void)
{
  m_notifierEnabled = false;
}
void 
MobilityHelper::SetPositionAllocator (Ptr<PositionAllocator> allocator)
{
  m_position = allocator;
}
void 
MobilityHelper::SetPositionAllocator (std::string type,
				      std::string n1, PValue v1,
				      std::string n2, PValue v2,
				      std::string n3, PValue v3,
				      std::string n4, PValue v4,
				      std::string n5, PValue v5,
				      std::string n6, PValue v6,
				      std::string n7, PValue v7,
				      std::string n8, PValue v8,
				      std::string n9, PValue v9)
{
  ObjectFactory pos;
  pos.SetTypeId (type);
  pos.Set (n1, v1);
  pos.Set (n2, v2);
  pos.Set (n3, v3);
  pos.Set (n4, v4);
  pos.Set (n5, v5);
  pos.Set (n6, v6);
  pos.Set (n7, v7);
  pos.Set (n8, v8);
  pos.Set (n9, v9);
  m_position = pos.Create ()->QueryInterface<PositionAllocator> ();
}

void 
MobilityHelper::SetMobilityModel (std::string type,
				  std::string n1, PValue v1,
				  std::string n2, PValue v2,
				  std::string n3, PValue v3,
				  std::string n4, PValue v4,
				  std::string n5, PValue v5,
				  std::string n6, PValue v6,
				  std::string n7, PValue v7,
				  std::string n8, PValue v8,
				  std::string n9, PValue v9)
{
  m_mobility.SetTypeId (type);
  m_mobility.Set (n1, v1);
  m_mobility.Set (n2, v2);
  m_mobility.Set (n3, v3);
  m_mobility.Set (n4, v4);
  m_mobility.Set (n5, v5);
  m_mobility.Set (n6, v6);
  m_mobility.Set (n7, v7);
  m_mobility.Set (n8, v8);
  m_mobility.Set (n9, v9);
}

std::string 
MobilityHelper::GetMobilityModelType (void) const
{
  return m_mobility.GetTypeId ().GetName ();
}

void 
MobilityHelper::Layout (const std::vector<Ptr<Object> > &objects)
{
  for (std::vector<Ptr<Object> >::const_iterator i = objects.begin (); i != objects.end (); i++)
    {
      Ptr<Object> object = *i;
      Ptr<MobilityModel> model = object->QueryInterface<MobilityModel> ();
      if (model == 0)
	{
	  model = m_mobility.Create ()->QueryInterface<MobilityModel> ();
	  if (model == 0)
	    {
	      NS_FATAL_ERROR ("The requested mobility model is not a mobility model: \""<< 
			      m_mobility.GetTypeId ().GetName ()<<"\"");
	    }
	  object->AddInterface (model);
	}
      Vector position = m_position->GetNext ();
      model->SetPosition (position);
      if (m_notifierEnabled)
	{
	  Ptr<MobilityModelNotifier> notifier = 
	    object->QueryInterface<MobilityModelNotifier> ();
	  if (notifier == 0)
	    {
	      object->AddInterface (CreateObject<MobilityModelNotifier> ());
	    }
	}
    }
}

} // namespace ns3
