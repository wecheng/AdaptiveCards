#pragma once

#include "AdaptiveCards.Rendering.Uwp.h"
#include "util.h"
#include "InputValue.h"
#include "AdaptiveInputs.h"

#define InspectableClassNoAddRef(runtimeClassName, trustLevel) \
public: \
    static const wchar_t* STDMETHODCALLTYPE InternalGetRuntimeClassName() throw() \
    { \
        static_assert((RuntimeClassT::ClassFlags::value & ::Microsoft::WRL::WinRtClassicComMix) == ::Microsoft::WRL::WinRt || \
                          (RuntimeClassT::ClassFlags::value & ::Microsoft::WRL::WinRtClassicComMix) == ::Microsoft::WRL::WinRtClassicComMix, \
                      "'InspectableClass' macro must not be used with ClassicCom clasess."); \
        static_assert(__is_base_of(::Microsoft::WRL::Details::RuntimeClassBase, RuntimeClassT), \
                      "'InspectableClass' macro can only be used with ::Windows::WRL::RuntimeClass types"); \
        static_assert(!__is_base_of(IActivationFactory, RuntimeClassT), \
                      "Incorrect usage of IActivationFactory interface. Make sure that your RuntimeClass doesn't implement IActivationFactory interface use ::Windows::WRL::ActivationFactory instead or 'InspectableClass' macro is not used on ::Windows::WRL::ActivationFactory"); \
        return runtimeClassName; \
    } \
    static TrustLevel STDMETHODCALLTYPE InternalGetTrustLevel() throw() { return trustLevel; } \
    STDMETHOD(GetRuntimeClassName)(_Out_ HSTRING * runtimeName) \
    { \
        *runtimeName = nullptr; \
        HRESULT hr = S_OK; \
        const wchar_t* name = InternalGetRuntimeClassName(); \
        if (name != nullptr) \
        { \
            hr = ::WindowsCreateString(name, static_cast<UINT32>(::wcslen(name)), runtimeName); \
        } \
        return hr; \
    } \
    STDMETHOD(GetTrustLevel)(_Out_ TrustLevel * trustLvl) \
    { \
        *trustLvl = trustLevel; \
        return S_OK; \
    } \
    STDMETHOD(GetIids) \
    (_Out_ ULONG * iidCount, \
     _When_(*iidCount == 0, _At_(*iids, _Post_null_)) _When_(*iidCount > 0, _At_(*iids, _Post_notnull_)) _Result_nullonfailure_ IID * *iids) \
    { \
        return RuntimeClassT::GetIids(iidCount, iids); \
    } \
    STDMETHOD(QueryInterface)(REFIID riid, _Outptr_result_nullonfailure_ void** ppvObject) \
    { \
        return RuntimeClassT::QueryInterface(riid, ppvObject); \
    } \
\
private:


namespace AdaptiveNamespace
{
    class DECLSPEC_UUID("F25E0D36-0B5B-4398-AFC8-F84105EC46A2") RenderedAdaptiveCard
        : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>,
                                              Microsoft::WRL::CloakedIid<ITypePeek>,
                                              Microsoft::WRL::Implements<ABI::AdaptiveNamespace::IRenderedAdaptiveCard>>
    {
        InspectableClassNoAddRef(RuntimeClass_AdaptiveCards_Rendering_Uwp_RenderedAdaptiveCard, BaseTrust);

    public:
        STDMETHOD_(ULONG, Release)();
        STDMETHOD_(ULONG, AddRef)();

        ULONG AddRefInternal();
        ULONG ReleaseInternal();

        RenderedAdaptiveCard();

        HRESULT RuntimeClassInitialize();
        HRESULT RuntimeClassInitialize(
            _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveError*>* errors,
            _In_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveWarning*>* warnings);

        // IRenderedAdaptiveCard
        IFACEMETHODIMP get_OriginatingCard(_COM_Outptr_ ABI::AdaptiveNamespace::IAdaptiveCard** value);
        IFACEMETHODIMP get_FrameworkElement(_COM_Outptr_ ABI::Windows::UI::Xaml::IFrameworkElement** value);
        IFACEMETHODIMP get_UserInputs(_COM_Outptr_ ABI::AdaptiveNamespace::IAdaptiveInputs** value);
        IFACEMETHODIMP add_Action(_In_ ABI::Windows::Foundation::ITypedEventHandler<ABI::AdaptiveNamespace::RenderedAdaptiveCard*,
                                                                                    ABI::AdaptiveNamespace::AdaptiveActionEventArgs*>* handler,
                                  _Out_ EventRegistrationToken* token);
        IFACEMETHODIMP remove_Action(_In_ EventRegistrationToken token);

        IFACEMETHODIMP add_MediaClicked(
            _In_ ABI::Windows::Foundation::ITypedEventHandler<ABI::AdaptiveNamespace::RenderedAdaptiveCard*, ABI::AdaptiveNamespace::AdaptiveMediaEventArgs*>* handler,
            _Out_ EventRegistrationToken* token);
        IFACEMETHODIMP remove_MediaClicked(_In_ EventRegistrationToken token);

        IFACEMETHODIMP get_Errors(_COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveError*>** value);
        IFACEMETHODIMP get_Warnings(
            _COM_Outptr_ ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveWarning*>** value);

        // ITypePeek method
        void* PeekAt(REFIID riid) override { return PeekHelper(riid, this); }

        HRESULT AddInputValue(ABI::AdaptiveNamespace::IAdaptiveInputValue* inputValue);
        void SetFrameworkElement(ABI::Windows::UI::Xaml::IFrameworkElement* value);
        void SetOriginatingCard(ABI::AdaptiveNamespace::IAdaptiveCard* value);
        HRESULT SendActionEvent(ABI::AdaptiveNamespace::IAdaptiveActionElement* eventArgs);
        HRESULT SendMediaClickedEvent(ABI::AdaptiveNamespace::IAdaptiveMedia* eventArgs);


    private:
        Microsoft::WRL::ComPtr<ABI::AdaptiveNamespace::IAdaptiveCard> m_originatingCard;
        Microsoft::WRL::ComPtr<AdaptiveNamespace::AdaptiveInputs> m_inputs;
        Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::IFrameworkElement> m_frameworkElement;
        Microsoft::WRL::WeakRef m_weakFrameworkElement;
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveError*>> m_errors;
        Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IVector<ABI::AdaptiveNamespace::IAdaptiveWarning*>> m_warnings;
        std::shared_ptr<ActionEventSource> m_actionEvents;
        std::shared_ptr<MediaEventSource> m_mediaClickedEvents;
        ULONG m_cRefInternal;
    };

    ActivatableClass(RenderedAdaptiveCard);
}
