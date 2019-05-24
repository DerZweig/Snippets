#pragma once

#include "win32_definitions.h"

namespace my::win32
{
    struct window_class final
    {
        window_class() noexcept           = default;
        window_class(const window_class&) = delete;

        window_class(HINSTANCE hInstance, const string_view& classname, UINT style, const WNDPROC proc);
        ~window_class();
        window_class(window_class&& other) noexcept;

        window_class& operator=(window_class&& other) noexcept;
        window_class& operator=(const window_class&) = delete;

        [[nodiscard]] HINSTANCE     instance() const noexcept;
        [[nodiscard]] bool          registered() const noexcept;
        [[nodiscard]] ATOM          value() const noexcept;
        [[nodiscard]] const string& name() const;
    private:
        HINSTANCE m_instance{nullptr};
        string    m_classname{};
        ATOM      m_value{};
    };

    inline window_class::window_class(const HINSTANCE hInstance, const string_view& classname, const UINT style, const WNDPROC proc) : m_instance(hInstance),
                                                                                                                                       m_classname(classname)
    {
        WNDCLASSEX wndcls{};

        wndcls.cbSize        = sizeof(WNDCLASSEX);
        wndcls.style         = style;
        wndcls.hInstance     = m_instance;
        wndcls.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
        wndcls.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);
        wndcls.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wndcls.lpfnWndProc   = proc;
        wndcls.lpszClassName = m_classname.c_str();

        m_value = RegisterClassEx(&wndcls);
    }

    inline window_class::~window_class()
    {
        if (!registered()) return;

        UnregisterClass(m_classname.c_str(), m_instance);
        m_value = 0u;
        m_classname.clear();
    }

    inline window_class::window_class(window_class&& other) noexcept : m_instance(other.m_instance),
                                                                       m_classname(std::move(other.m_classname)),
                                                                       m_value(other.m_value)
    {
        other.m_instance = nullptr;
        other.m_value    = 0;
    }

    inline window_class& window_class::operator=(window_class&& other) noexcept
    {
        std::swap(m_instance, other.m_instance);
        std::swap(m_classname, other.m_classname);
        std::swap(m_value, other.m_value);

        return *this;
    }

    inline HINSTANCE window_class::instance() const noexcept
    {
        return m_instance;
    }

    inline bool window_class::registered() const noexcept
    {
        return m_value != 0u;
    }

    inline ATOM window_class::value() const noexcept
    {
        return m_value;
    }

    inline const string& window_class::name() const
    {
        return m_classname;
    }
}
