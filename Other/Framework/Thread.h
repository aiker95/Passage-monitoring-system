#ifndef _THREAD_H_
#define _THREAD_H_

#include "ErrorType.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
/// @class CThread
/// @brief class for the execution function(s) in the individual thread
//////////////////////////////////////////////////////////////////////////

namespace UFramework {

	typedef CF_ErrorType (*ThreadFunction)(void * Context);

	
	class CF_Thread 
	{
	public:

		enum Status {

			Idle,   /// after SetFunc(), Wait() or Terminate()
			Work,   /// after Start() (just before the set function execution)
			Stopped, /// just after the end of the set function execution
			Suspended, /// unused (internal error)
			Unknown /// before SetFunc() (after construction)
		};
		
		
		CF_Thread(const char * Name = 0);			/// Default constructor	
		
		
		virtual ~CF_Thread();	/// Destructor
		
		
        //---------------------------------------------------------------------
        /// @brief Sets pointer to function to execute within the thread (returned value will be ignored)
        /// @return ErrorOK (always)
        CF_ErrorType SetThreadFunction( ThreadFunction functionPtr );


        //---------------------------------------------------------------------
        /// @brief Start - execute the "thread function" (see: SetThreadFunction)
		///     @param[in] Context - will be passed into the "thread function"
		/// @return Error code
        ///     @retval if (Status == Stopped) then ErrorOK (and set Status = Work)
        ///             if (Status == Work)    then ERR_THREAD_ALREADY_RUNNING
        ///                                    else ERR_THREAD_START
        CF_ErrorType Start( void * Context );	


        //---------------------------------------------------------------------
        /// @brief Stop - set event to invite to stop thread execution
		/// @return Error code
        ///     @retval if (Status == Work)    then ErrorOK or ERR_OS_DEPEND_FAILED
        ///                                    else ERR_THREAD_NOT_RUNNING
        CF_ErrorType Stop();


        //---------------------------------------------------------------------
        /// @brief Terminate - Terminates thread and set Status = Unknown (ends the thread! not just the current function execution)
        /// @return ErrorOK (always)
        CF_ErrorType Terminate();


        //---------------------------------------------------------------------
        /// @brief Wait - Waits for the (Status != Work)
		/// @param[in] timeout - timeout of wait
		/// @return Error code
		///   @retval if (Status == Work) then ERR_TIMEOUT
        ///                               else ErrorOK
        CF_ErrorType Wait( unsigned long timeout );


        //---------------------------------------------------------------------
		/// @brief GetStatus - Get actual status
		///
		/// @return status:
		///   @retval Idle
		///   @retval Work
		///   @retval Stopped	
		///   @retval Suspend
		///   @retval Unknown		
        Status GetStatus() const;

		
	protected:
		
		HANDLE m_hThread;
		volatile void * m_context;
		volatile ThreadFunction m_functionPtr;
		volatile Status m_status;
		
		static DWORD __stdcall threadFunction(void * Context);			
	};

}

#endif
