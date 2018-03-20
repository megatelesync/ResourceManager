#include "../H/Utils_RenderingTester.h"

namespace Dv
{
	namespace d3d_utils
	{
		namespace RenderingTester
		{
			/**************************************************************************************************************************
			* Fwd delcarations
			**************************************************************************************************************************/
			void                      OverrideRenderState_IfShouldDoIt     (RS_RenderState *pRS, CodePoint *pInCodePoint, RenderingTesterMain *pInTesterMain);
			void                      OverrideRenderState_BySettingIt      (CodePoint *pInCodePoint, RenderingTesterMain *pInTesterMain);

			bool                      ShouldOverrideRenderState            (CodePoint *pInCodePoint, RenderingTesterMain *pInTesterMain);
			void                      OverrideRenderState                  (CodePoint *pInCodePoint, RenderingTesterMain *pInTesterMain);

			/**************************************************************************************************************************
			* RenderingTesterMain
			**************************************************************************************************************************/
			void RenderingTesterMain::Initialize(const Settings *pInSettings, Loaders::ResManD3D11_Main *pInResMan)
			{
				assert(pInResMan);
				_settings = *pInSettings; 
				_pResMan = pInResMan;
			}

			void RenderingTesterMain::Tick()
			{
				_tickIndex++;
			}

			CodePoint *RenderingTesterMain::SET_AND_EXECUTE_CODE_POINT(const std::string& InName, CodePoint *pNullOrCodePoint)
			{
				auto it_point = _codePoints.find(InName);
				if (pNullOrCodePoint == nullptr)
				{
					if(it_point == _codePoints.end())
					{
						it_point = _codePoints.insert(std::make_pair(InName, pNullOrCodePoint));
					}
				}

				// now it_point points to registered code point
				_ProcessCodePoint(it_point);
			}

			void RenderingTesterMain::SetCodePoint_RSSnapshot(const std::string& InName)
			{
				_codePointName_RSSnapshot = InName;
				auto pCodePoint = GetCodePointByName(InName);
				pCodePoint->SetShouldSnapshotRSHere(this, true);
			}
			void RenderingTesterMain::SetCodePoint_Render(const std::string& InName)
			{
				_codePointName_Render = InName; 
				auto pCodePoint = GetCodePointByName(InName);
				pCodePoint->SetShouldDraw(this, true);
			}

			void RenderingTesterMain::_ProcessCodePoint(CodepointMap::iterator it_point)
			{
				auto pCodePoint = &(it_point->second);
				Util::NotYetImplVoid("RenderingTesterMain::_ProcessCodePoint");
				// Shoulf we snapshot RS because we want to use it as render state later
				bool bShouldSnapshotRS_toMove = (pCodePoint->ShouldSnapshotRSHere() && (false == pCodePoint->ShouldDrawHere()));
				bool bShouldSnapshotRSHere = pCodePoint->GetSettings().bShouldSnapshotRS || bShouldSnapshotRS_toMove;
				if (bShouldSnapshotRSHere)
				{
					RS_RenderState rs;
					SnapshotDeviceRenderState(GetDevCon(), &rs);
					pCodePoint->SetSnapshottedRS(this, &rs);
				}
				// Should we move RS from some other sorce codepoint or not?
				bool bShouldMoveRS = IsSnapshotCodePointSetted() && false == pCodePoint->ShouldSnapshotRSHere();
				if(pCodePoint->ShouldDrawHere())
				{ 
					if (bShouldMoveRS)
					{
						auto pRSSnapshotCodePoint = GetCodePointByName(GetCodePointName_RSSnapshot());
						RS_RenderState saved_rs = *(pRSSnapshotCodePoint->GetSnapshottedRS());

						// Update RS if should do it
						OverrideRenderState_IfShouldDoIt(&saved_rs, pCodePoint, this);
						SetRS(GetDevCon(), pCodePoint->GetSnapshottedRS());
					}
					else
					{
						// Here we should only override RS by setting it to context directly
						OverrideRenderState_BySettingIt(pCodePoint, this);
					}

					_PerformRenderClearAndPresent_IfShouldDoIt(pCodePoint);
				}
			}

			void RenderingTesterMain::_PerformRenderClearAndPresent_IfShouldDoIt(CodePoint *pInCodePoint)
			{
				// Clear render targets and depth stencils if necessary
				if (_settings.bClearDepth)
				{
					UINT clearFlags = 0;
					if (_settings.bClearDepth)
					{
						clearFlags |= D3D11_CLEAR_DEPTH;
					}
					if (_settings.bClearStencil)
					{
						clearFlags |= D3D11_CLEAR_STENCIL;
					}
					GetDevCon()->ClearDepthStencilView(GetDepthStencilView(), clearFlags, _settings.depthStencilClearDepth, _settings.depthStencilClearStencil);
				}

				if (_settings.bClearRenderTarget)
				{
					GetDevCon()->ClearRenderTargetView(GetRenderTargetView(), _settings.renderTargetClearColor);
				}

				// Perform real draw calls
				_PerformRealRendering(pInCodePoint);

				// Present if necessary
				if (_settings.bPresent)
				{
					GetSwapChain()->Present(0, 0);
				}
			}

			void RenderingTesterMain::_PerformRealRendering(CodePoint *pInCodePoint)
			{
				Util::NotYetImplVoid("RenderingTesterMain::_PerformRealRendering");
			}

			const CodePoint *RenderingTesterMain::GetCodePointByName(const std::string& InName) const
			{
				return &(_codePoints[InName]);
			}
			CodePoint *RenderingTesterMain::GetCodePointByName(const std::string& InName)
			{
				return &(_codePoints[InName]);
			}

			bool RenderingTesterMain::IsSnapshotCodePointSetted() const
			{
				bool bNotEmpty = _codePointName_RSSnapshot.size() > 0;
				return bNotEmpty;
			}

			/*************************************************************************************************************************
			* Settings utils
			*************************************************************************************************************************/
			bool ShouldLogCodePoint_ThisTick(const CodePoint *pInCodePoint, const LoggingSettings *pSettings, int InTickIndex)
			{
				if (false == ShouldLogCodePoint(pInCodePoint, pSettings))
				{
					return false;
				}
				if (0 != (InTickIndex % pSettings->minCountOfFramesToLogCodePointOnce))
				{
					return false;
				}
				if (pInCodePoint->GetNumTimesExecuted() > 0)
				{
					if (pSettings->bMasterCodePoint_LogOnlyOnce)
					{
						return false;
					}

					if (pSettings->bCodePoint_LogOnce_Rendering && pInCodePoint->ShouldDrawHere())
					{
						return false;
					}

					if (pSettings->bCodePoint_LogOnce_RenderState && pInCodePoint->ShouldSnapshotRSHere())
					{
						return false;
					}
				}
				return true;
			}
			bool ShouldLogCodePoint(const CodePoint *pInCodePoint, const LoggingSettings *pSettings)
			{
				assert(pInCodePoint);
				assert(pSettings);
				if (false == pSettings->bMasterEnableLogging)
				{
					return false;
				}
				if (false == pSettings->bMasterLogCodePoints)
				{
					return false;
				}
				if (pSettings->bMasterLogCodePoints)
				{
					return true;
				}
				Util::NotYetImplVoid("Implement other flags");
				return true;
			}

			/**************************************************************************************************************************
			* CodePoint implementation
			**************************************************************************************************************************/
			CodePoint::CodePoint(const std::string& InName) :
				_name(InName)
			{
			}
		}
	}
}