//=============================================================================
// Copyright © 2017 FLIR Integrated Imaging Solutions, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of FLIR
// Integrated Imaging Solutions, Inc. ("Confidential Information"). You
// shall not disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with FLIR Integrated Imaging Solutions, Inc. (FLIR).
//
// FLIR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. FLIR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

#ifndef PGR_SPINNAKER_INTERFACE_H
#define PGR_SPINNAKER_INTERFACE_H

#include "SpinnakerPlatform.h"
#include "SpinnakerDefs.h"
#include "CameraList.h"
#include "Event.h"
#include "TransportLayerInterface.h"


namespace Spinnaker
{
	/**
	 * @defgroup SpinnakerClasses Spinnaker Classes
	 */

	/*@{*/

	/**
	 * @defgroup Interface_h Interface Class
	 */

	/*@{*/

    /**
    * @brief An interface object which holds a list of cameras.
    */

	class SPINNAKER_API Interface
	{
	public:

		/**
		* Virtual Destructor
		*/
		virtual ~Interface(void);


		/**
         * Returns a list of cameras available on this interface.  This call
		 * returns either usb3 vision  or gige vision cameras depending on the
		 * underlying transport layer of this interface.  The camera list object
		 * will reference count the cameras that it holds.  It is important that
		 * the CameraList is destroyed or is cleared before System::ReleaseInstance()
		 * can be called or an InterfaceList that holds this interface can be cleared.
		 *
		 * @see System::ReleaseInstance()
		 *
		 * @see InterfaceList::Clear()
		 *
		 * @see CameraList::Clear()
         *
         * @param updateCameras A flag used to issue an updateCameras() call internally 
         *        before getting the camera list
         *
         * @return An CameraList object that contains a list of cameras on this interface.
         */
		CameraList GetCameras(bool updateCameras = true) const;

		/**
         * Updates the list of cameras on this interface.  This function needs to be called
		 * before any cameras can be discovered using GetCameras().  System::GetCameras() will
		 * automatically call this function for each interface it enumerates.  If the list 
         * changed after the last time System::GetCameras() or UpdateCameras() was called 
         * then the return value will be true, otherwise it is false.
		 *
		 * @see System::GetCameras()
		 *
		 * @see GetCameras()
         *
         * @return true if cameras changed on interface and false otherwise.
         */
		bool UpdateCameras();

		/**
		 * Gets a nodeMap that is generated from a GenICam
		 * XML file for the GenTL interface Module.
		 *
		 * @return  A reference to a INodeMap object.
		 */
		GenApi::INodeMap & GetTLNodeMap() const;

		/**
		* Registers an event for the interface
        *
        * @param evtToRegister The event to register for the interface        
        *
		*/
        void RegisterEvent(Event & evtToRegister);

		/**
		* Unregisters an event for the interface
        *
        * @param evtToUnregister The event to unregister from the interface
        *
		*/
		void UnregisterEvent(Event & evtToUnregister);

		/**
		* Checks if the interface is in use by any camera objects
		*
        * @return Returns true if the interface is in use and false otherwise.
		*/
		bool IsInUse() const;

		/**
		* Broadcast an Action Command to all devices on interface
		*
		* @param deviceKey The Action Command's device key
		* @param groupKey The Action Command's group key
		* @param groupMask The Action Command's group mask
		* @param actionTime (Optional) Time when to assert a future action. Zero means immediate action.
		* @param pResultSize (Optional) The number of results in the results array. The value passed should be equal to the expected number of devices that acknowledge the command. Returns the number of received results.
		* @param results (Optional) An Array with *pResultSize elements to hold the action command result status. The buffer is filled starting from index 0. If received results are less than expected number of devices that acknowledge the command, remaining results are not changed. If received results are more than expected number of devices that acknowledge the command, extra results are ignored and not appended to array. This parameter is ignored if pResultSize is 0. Thus this parameter can be NULL if pResultSize is 0 or NULL.
		*
		*/
		void SendActionCommand(unsigned int deviceKey, unsigned int groupKey, unsigned int groupMask, unsigned long long actionTime = 0, unsigned int* pResultSize = 0, ActionCommandResult results[] = NULL) const;

	public:

		/*
		* Gets vital interface information without connecting to the XML through
		* transport layer specific bootstrap registers.
		*/
		TransportLayerInterface TLInterface;
        
	protected:
		friend class InterfaceInternal;
		friend class SystemImpl;
		struct InterfaceData; // Forward declaration
		InterfaceData* m_pInterfaceData;

	private:
		/*
		* Default Constructor
		*/
		Interface(void);

		/*
		* Copy Constructor
		*/
		Interface(const Interface & /*iface*/);

		/**
		* Assignment operator.
		*/
		Interface&	operator=(const Interface&  /*iface*/);
	};

	/*@}*/

	/*@}*/
}

#endif // PGR_SPINNAKER_INTERFACE_H