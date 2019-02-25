#include "pch.h"

#include "AdaptiveRenderContext.h"
#include "AdaptiveError.h"
#include "AdaptiveWarning.h"
#include "InputValue.h"
#include "Util.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AdaptiveNamespace;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;

namespace AdaptiveNamespace
{
    HRESULT AdaptiveRenderContext::RuntimeClassInitialize() noexcept { return S_OK; }

    HRESULT AdaptiveRenderContext::RuntimeClassInitialize(_In_ IAdaptiveHostConfig* hostConfig,
                                                          _In_ IAdaptiveElementRendererRegistration* elementRendererRegistration,
                                                          _In_ IAdaptiveActionRendererRegistration* actionRendererRegistration,
                                                          _In_ IAdaptiveCardResourceResolvers* resourceResolvers,
                                                          _In_ IResourceDictionary* overrideDictionary,
                                                          _In_ IResourceDictionary* defaultActionSentimentStyles,
                                                          _In_ RenderedAdaptiveCard* renderResult) noexcept try
    {
        m_hostConfig = hostConfig;
        m_elementRendererRegistration = elementRendererRegistration;
        m_actionRendererRegistration = actionRendererRegistration;
        m_resourceResolvers = resourceResolvers;
        m_overrideDictionary = overrideDictionary;
        m_actionSentimentDefaultDictionary = defaultActionSentimentStyles;

        ComPtr<RenderedAdaptiveCard> strongRenderResult = renderResult;
        RETURN_IF_FAILED(strongRenderResult.AsWeak(&m_weakRenderResult));

        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveActionInvoker>(&m_actionInvoker, renderResult));
        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveMediaEventInvoker>(&m_mediaEventInvoker, renderResult));

        return S_OK;
    }
    CATCH_RETURN;

    HRESULT AdaptiveRenderContext::get_HostConfig(_COM_Outptr_ IAdaptiveHostConfig** value)
    {
        return m_hostConfig.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_ElementRenderers(_COM_Outptr_ IAdaptiveElementRendererRegistration** value)
    {
        return m_elementRendererRegistration.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_ActionRenderers(_COM_Outptr_ IAdaptiveActionRendererRegistration** value)
    {
        return m_actionRendererRegistration.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_ActionInvoker(_COM_Outptr_ IAdaptiveActionInvoker** value)
    {
        return m_actionInvoker.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_MediaEventInvoker(_COM_Outptr_ IAdaptiveMediaEventInvoker** value)
    {
        return m_mediaEventInvoker.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_ResourceResolvers(_COM_Outptr_ IAdaptiveCardResourceResolvers** value)
    {
        return m_resourceResolvers.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::get_OverrideStyles(_COM_Outptr_ IResourceDictionary** overrideDictionary)
    {
        return m_overrideDictionary.CopyTo(overrideDictionary);
    }

    HRESULT AdaptiveRenderContext::AddError(ABI::AdaptiveNamespace::ErrorStatusCode statusCode, _In_ HSTRING message)
    {
        ComPtr<AdaptiveError> error;
        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveError>(&error, statusCode, message));
        ComPtr<IVector<ABI::AdaptiveNamespace::AdaptiveError*>> errors;
        ComPtr<RenderedAdaptiveCard> renderResult;
        RETURN_IF_FAILED(GetRenderResult(renderResult.GetAddressOf()));
        RETURN_IF_FAILED(renderResult->get_Errors(&errors));
        return (errors->Append(error.Detach()));
    }

    HRESULT AdaptiveRenderContext::AddWarning(ABI::AdaptiveNamespace::WarningStatusCode statusCode, _In_ HSTRING message)
    {
        ComPtr<AdaptiveWarning> warning;
        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveWarning>(&warning, statusCode, message));
        ComPtr<IVector<ABI::AdaptiveNamespace::AdaptiveWarning*>> warnings;
        ComPtr<RenderedAdaptiveCard> renderResult;
        RETURN_IF_FAILED(GetRenderResult(renderResult.GetAddressOf()));
        RETURN_IF_FAILED(renderResult->get_Warnings(&warnings));
        return (warnings->Append(warning.Detach()));
    }

    HRESULT AdaptiveRenderContext::get_CardFrameworkElement(_COM_Outptr_ ABI::Windows::UI::Xaml::IFrameworkElement** value)
    {
        return m_cardFrameworkElement.CopyTo(value);
    }

    HRESULT AdaptiveRenderContext::put_CardFrameworkElement(_In_ ABI::Windows::UI::Xaml::IFrameworkElement* value)
    {
        m_cardFrameworkElement = value;
        return S_OK;
    }

    HRESULT AdaptiveRenderContext::AddInputValue(_In_ IAdaptiveInputValue* inputValue)
    {
        ComPtr<RenderedAdaptiveCard> renderResult;
        RETURN_IF_FAILED(GetRenderResult(renderResult.GetAddressOf()));
        RETURN_IF_FAILED(renderResult == nullptr ? E_NOINTERFACE : S_OK);
        return renderResult->AddInputValue(inputValue);
    }

    Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::IResourceDictionary> AdaptiveRenderContext::GetDefaultActionSentimentDictionary()
    {
        return m_actionSentimentDefaultDictionary;
    }

    HRESULT AdaptiveRenderContext::GetRenderResult(AdaptiveCards::Rendering::Uwp::RenderedAdaptiveCard** renderResult)
    {
        ComPtr<IRenderedAdaptiveCard> strongRenderResult;
        RETURN_IF_FAILED(m_weakRenderResult.As(&strongRenderResult));
        RETURN_IF_FAILED(strongRenderResult == nullptr ? E_FAIL : S_OK);
        ComPtr<RenderedAdaptiveCard> renderResultObject = PeekInnards<RenderedAdaptiveCard>(strongRenderResult);
        RETURN_IF_FAILED(renderResultObject == nullptr ? E_FAIL : S_OK);
        return renderResultObject.CopyTo(renderResult);
    }
}
