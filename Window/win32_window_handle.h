#pragma once

#include "win32_definitions.h"
#include <functional>

namespace my::win32
{
    struct window_handle final
    {
        using param_pointer = std::unique_ptr<void, std::function<void(void*)>>;

        window_handle() noexcept            = default;
        window_handle(const window_handle&) = delete;

        window_handle(DWORD              extendedStyle,
                      const string_view& classname,
                      const string_view& windowName,
                      DWORD              style,
                      int                x,
                      int                y,
                      int                width,
                      int                height,
                      HWND               parent,
                      HMENU              menu,
                      HINSTANCE          hInstance);

        window_handle(DWORD              extendedStyle,
                      const string_view& classname,
                      const string_view& windowName,
                      DWORD              style,
                      int                x,
                      int                y,
                      int                width,
                      int                height,
                      HWND               parent,
                      HMENU              menu,
                      HINSTANCE          hInstance,
                      param_pointer&&    param);

        ~window_handle();
        window_handle(window_handle&& other) noexcept;

        window_handle& operator=(window_handle&& other) noexcept;
        window_handle& operator=(const window_handle&) = delete;

        [[nodiscard]] bool empty() const noexcept;

        // ReSharper disable CppNonExplicitConversionOperator
        operator HWND() const noexcept;
        // ReSharper restore CppNonExplicitConversionOperator
    private:
        param_pointer m_data{};
        HWND          m_value{nullptr};
    };

    inline window_handle::window_handle(const DWORD        extendedStyle,
                                        const string_view& classname,
                                        const string_view& windowName,
                                        const DWORD        style,
                                        const int          x,
                                        const int          y,
                                        const int          width,
                                        const int          height,
                                        HWND               parent,
                                        HMENU              menu,
                                        HINSTANCE          hInstance)
    {
        m_value = CreateWindowEx(extendedStyle,
                                 classname.data(),
                                 windowName.data(),
                                 style,
                                 x, y,
                                 width, height,
                                 parent,
                                 menu,
                                 hInstance,
                                 nullptr);
    }

    inline window_handle::window_handle(const DWORD        extendedStyle,
                                        const string_view& classname,
                                        const string_view& windowName,
                                        const DWORD        style,
                                        const int          x,
                                        const int          y,
                                        const int          width,
                                        const int          height,
                                        HWND               parent,
                                        HMENU              menu,
                                        HINSTANCE          hInstance,
                                        param_pointer&&    param)
    {
        m_data  = std::move(param);
        m_value = CreateWindowEx(extendedStyle,
                                 classname.data(),
                                 windowName.data(),
                                 style,
                                 x, y,
                                 width, height,
                                 parent,
                                 menu,
                                 hInstance,
                                 m_data.get());
    }

    inline window_handle::~window_handle()
    {
        if (empty()) return;

        DestroyWindow(m_value);
        m_value = nullptr;
    }

    inline window_handle::window_handle(window_handle&& other) noexcept : m_data(std::move(other.m_data)),
                                                                          m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    inline window_handle& window_handle::operator=(window_handle&& other) noexcept
    {
        std::swap(m_data, other.m_data);
        std::swap(m_value, other.m_value);
        return *this;
    }

    inline bool window_handle::empty() const noexcept
    {
        return m_value == nullptr;
    }

    inline window_handle::operator HWND() const noexcept
    {
        return m_value;
    }
}
