#include "pch.h"

#include "AdaptiveOpenUrlAction.h"
#include "AdaptiveOpenUrlActionRenderer.h"
#include "Util.h"
#include "AdaptiveElementParserRegistration.h"
#include "XamlBuilder.h"

using namespace ABI::AdaptiveNamespace;
using namespace Microsoft::WRL;

namespace AdaptiveNamespace
{
    HRESULT AdaptiveOpenUrlActionRenderer::RuntimeClassInitialize() noexcept try
    {
        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveOpenUrlActionRenderer::Render(_In_ IAdaptiveActionElement* action,
                                                _In_ IAdaptiveRenderContext* renderContext,
                                                _In_ IAdaptiveRenderArgs* renderArgs,
                                                _COM_Outptr_ ABI::Windows::UI::Xaml::IUIElement** result) noexcept try
    {
        XamlBuilder::BuildAction(action, renderContext, renderArgs, result);
        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveOpenUrlActionRenderer::FromJson(
        _In_ ABI::Windows::Data::Json::IJsonObject* jsonObject,
        _In_ ABI::AdaptiveNamespace::IAdaptiveElementParserRegistration* elementParserRegistration,
        _In_ ABI::AdaptiveNamespace::IAdaptiveActionParserRegistration* actionParserRegistration,
        _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::AdaptiveWarning*>* adaptiveWarnings,
        _COM_Outptr_ ABI::AdaptiveNamespace::IAdaptiveActionElement** element) noexcept try
    {
        return AdaptiveNamespace::FromJson<AdaptiveNamespace::AdaptiveOpenUrlAction, AdaptiveSharedNamespace::OpenUrlAction, AdaptiveSharedNamespace::OpenUrlActionParser>(
            jsonObject, elementParserRegistration, actionParserRegistration, adaptiveWarnings, element);
    }
    CATCH_RETURN;
}
