#pragma once

#include <string>
#include <boost/optional.hpp>
#include "Log/Util_LoggingDevice.h"
#include "ResManD3D11/ResManD3D11.H"
#include "../../!RS/H/RS_RenderState.h"

namespace Dv
{
	namespace d3d_utils
	{
		namespace RenderingTester
		{
			namespace ResMan                                = ::Dv::ResourceManager::Ver_0_0_0_0;
			namespace Loaders                               = ::Dv::ResManLoadersD3D11::Ver_0_0_0_0;


			/**************************************************************************************************************************
			* Params of the code point logging
			**************************************************************************************************************************/
			struct CodePointLogSettings
			{
				// if false logging of this codepoint is disabled entirely
				bool                                      bEnabled                  = true;
			};

			/**************************************************************************************************************************
			* Params of code point 
			**************************************************************************************************************************/
			struct CodePointSettings
			{
				bool                                      bIsActive                 = true;

				// true if RS should be snapshotted at this codepoint
				// (not necessarily as active RS codepoint)
				bool                                      bShouldSnapshotRS         = false;
			};

			/**************************************************************************************************************************
			* Some point of the code where render state is to be snapshotted or test drawing is to be performed.
			**************************************************************************************************************************/
			class CodePoint
			{
			public:
				/**********************************************************************************************************************
				* Construction
				**********************************************************************************************************************/
				CodePoint                                                            (const std::string& InName);

				/**********************************************************************************************************************
				* Properties
				**********************************************************************************************************************/
				const std::string&                          GetName                  () const                                                                         { return _name; }
				std::string&                                GetName                  ()                                                                               { return _name; }

				bool                                        ShouldSnapshotRSHere     () const                                                                         { return _bSnapshotRSHere; }
				bool                                        ShouldDrawHere           () const                                                                         { return _bDrawHere; }

				const RS_RenderState                        *GetSnapshottedRS        () const                                                                         { return &_snapshottedRenderState; }
				void                                         SetSnapshottedRS        (RenderingTesterMain *pInRenderingTester, const RS_RenderState *pInRS)           { _snapshottedRenderState = *pInRS; }

				void                                        SetShouldSnapshotRSHere  (RenderingTesterMain *pInRenderingTester, bool bInValue)                         { _bSnapshotRSHere = bInValue; }
				void                                        SetShouldDraw            (RenderingTesterMain *pInRenderingTester, bool bInValue)                         { _bDrawHere = bInValue; }

				// Number of times this code point is executed (meaning cs:ip pointed to the point)
				int                                         GetNumTimesExecuted      () const                { return _numTimesExecuted; }

				void                                        Notify_Executed          () { _numTimesExecuted++; }

				/**********************************************************************************************************************
				* Settings
				**********************************************************************************************************************/
				const CodePointLogSettings                 *GetLogSettings           () const { &_logSettings; }
				const CodePointSettings                    *GetSettings              () const { return &_settings; }

			private:
				std::string                                 _name;
				int                                         _numTimesExecuted;
				bool                                        _bSnapshotRSHere;
				bool                                        _bDrawHere;
				CodePointSettings                           _settings;
				CodePointLogSettings                        _logSettings;
				RS_RenderState                              _snapshottedRenderState;
			};
			
			/*************************************************************************************************************************
			* Settings of the logging
			*
			* WARNING!!! Master flags always override other flags!
			*************************************************************************************************************************/
			struct LoggingSettings
			{
				/******************************************************************************************************************
				* Settings of the logging device
				******************************************************************************************************************/
				Util::LogSettings                          loggingDevice;

				/******************************************************************************************************************
				* Master flag
				******************************************************************************************************************/
				// If false, any logging from the rendering tester is to be omitted
				bool                                       bMasterEnableLogging                                      = true;

				/******************************************************************************************************************
				* Code points: master flags
				******************************************************************************************************************/
				// If false, code points should not be logged at all. Period.
				bool                                       bMasterLogCodePoints                                      = true;

				// If true, all code points to be logged when gained
				// (ever if logging is turned off for particular code point)
				// Otherwise logs only active code points.
				bool                                       bMasterLogAllCodePoints                                   = false;

				/******************************************************************************************************************
				* Code points: which code points to log
				******************************************************************************************************************/
				bool                                       bLogOnlyRenderingCodePoints                               = false;
				bool                                       bLogOnlyRenderStateCodePoints                             = false;

				/******************************************************************************************************************
				* Code points: when to log
				******************************************************************************************************************/
				// each this count of frames logging is to be performed, and NOT more frequently
				int                                       minCountOfFramesToLogCodePointOnce                          = 1;
				// True if logging is always to be performed only once for any code point
				// (for any code points - render state snapshots of rendering)
				bool                                      bMasterCodePoint_LogOnlyOnce                                = false;
				// If true, all render state snapshot codepoints are to be logged only once
				bool                                      bCodePoint_LogOnce_RenderState                              = false;
				// If true, all rendering codepoints are to be logged only once
				bool                                      bCodePoint_LogOnce_Rendering                                = false;

				/******************************************************************************************************************
				* Code points: What to log
				******************************************************************************************************************/
				// If true, full render state information is to be logged for snapshots (if possible)
				bool                                      bCodePoint_OutputRenderStateSnapshotRSInfo                   = true;
				bool                                      bCodePoint_OutputRenderingRSInfo                             = true;
			};
			// Should we log the given code point this tick
			bool                                          ShouldLogCodePoint_ThisTick                  (const CodePoint *pInCodePoint, const LoggingSettings *pSettings, int InTickIndex);
			// Should we log the given code point at all (at some tick, not necessarily current tick)
			bool                                          ShouldLogCodePoint                           (const CodePoint *pInCodePoint, const LoggingSettings *pSettings);

			/*************************************************************************************************************************
			* Settings of the tester
			*************************************************************************************************************************/
			struct Settings
			{
				LoggingSettings                            logging;

				// if true, test rendering is enabled
				bool                                       bRenderingEnabled           = false;
				
				// true if clearing of Depth buffer is to be performed before the rendering
				bool                                       bClearDepth                 = true;
				// true if clearing of Stencil buffer is to be performed before the rendering
				bool                                       bClearStencil = true;

				FLOAT                                      depthStencilClearDepth      = 1.0F;
				UINT8                                      depthStencilClearStencil    = 0.0F;
				
				// true if render targets are to be cleared before the rendering
				bool                                       bClearRenderTarget          = true;
				float                                      renderTargetClearColor[4]   = { 1.0F, 1.0F, 1.0F, 1.0F };

				// true if presenting of frame is to be performed after the rendering
				bool                                       bPresent                    = true;

				// true if render state overriding enabled
				bool                                       bOverride_RenderState       = true;
			};

			/*************************************************************************************************************************
			* Main class of the rendering tester.
			*************************************************************************************************************************/
			class RenderingTesterMain
			{
			public:
				/*********************************************************************************************************************
				* Typedefs
				*********************************************************************************************************************/
				using CodepointMap                          = std::map<std::string, CodePoint>;

				/*********************************************************************************************************************
				* Initialization
				*********************************************************************************************************************/
				void                                        Initialize                                  (const Settings *pInSettings, Loaders::ResManD3D11_Main *pInResMan);

				/*********************************************************************************************************************
				* Ticking: necessary to count frames
				*********************************************************************************************************************/
				void                                        Tick                                        ();
				int                                         GetTickIndex                                () const                                                   { return _tickIndex; }

				/*********************************************************************************************************************
				* Settings
				*********************************************************************************************************************/
				const Settings                             *GetSettings                                 () const                                                   { return &_settings; }

				/*********************************************************************************************************************
				* Code points
				*********************************************************************************************************************/
				// Call this function where you want to place a code point;
				// You should pass registered codepoint (or nullptr, if code point has not been registered yet).
				CodePoint                                  *SET_AND_EXECUTE_CODE_POINT                  (const std::string& InName, CodePoint *pNullOrCodePoint);

				bool                                        IsSnapshotCodePointSetted                   () const;
				const std::string&                          GetCodePointName_RSSnapshot                 () const                                                   { return _codePointName_RSSnapshot; }
				const std::string&                          GetCodePointName_Render                     () const                                                   { return _codePointName_Render; }
				const CodePoint                            *GetCodePointByName                          (const std::string& InName) const;
				CodePoint                                  *GetCodePointByName                          (const std::string& InName);

				// Setting codepoints:
				// Code points are to be registered by name 
				// (to make it possible to register code points before they're set in the source code,
				// so you can set and configure code points before they are executed)
				void                                        SetCodePoint_RSSnapshot                     (const std::string& InName);
				void                                        SetCodePoint_Render                         (const std::string& InName);

			private:
				IDXGISwapChain                              *GetSwapChain                               () const;
				ID3D11DeviceContext                         *GetDevCon                                  () const                              { return _pResMan->GetDevCon(); }
				ID3D11Device                                *GetDev                                     () const                              { return _pResMan->GetDev(); }

				// Get current depth stencil view in use
				ID3D11DepthStencilView                      *GetDepthStencilView                        () const;
				ID3D11RenderTargetView                      *GetRenderTargetView                        () const;

				void                                        _ProcessCodePoint                           (CodepointMap::iterator it_point);
				void                                        _PerformRenderClearAndPresent_IfShouldDoIt  (CodePoint *pInCodePoint);
				void                                        _PerformRealRendering                       (CodePoint *pInCodePoint);

				CodepointMap                                _codePoints;
				std::string                                 _codePointName_RSSnapshot;
				std::string                                 _codePointName_Render;

				int                                         _tickIndex                                  = 0;
				Loaders::ResManD3D11_Main                  *_pResMan                                    = nullptr;

				Settings                                    _settings;
			};
		}
	}
}