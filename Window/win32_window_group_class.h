#pragma once
#include "win32_definitions.h"
#include "win32_window_class.h"
#include "win32_window_handle.h"
#include <functional>

namespace my::win32
{
    struct window_group_class
    {
        using value_type = std::function<bool(HWND, UINT, WPARAM, LPARAM)>;
        using pointer = value_type *;

        window_group_class() noexcept                 = default;
        window_group_class(const window_group_class&) = delete;

        window_group_class(HINSTANCE hInstance, const string_view& classname, UINT style);
        ~window_group_class() = default;
        window_group_class(window_group_class&& other) noexcept;

        window_group_class& operator=(window_group_class&& other) noexcept;
        window_group_class& operator=(const window_group_class&) = delete;

        [[nodiscard]] bool registered() const noexcept;
        [[nodiscard]] ATOM value() const noexcept;

        window_handle create_window(DWORD              extendedStyle,
                                    const string_view& windowName,
                                    DWORD              style,
                                    int                x,
                                    int                y,
                                    int                width,
                                    int                height,
                                    HWND               parent,
                                    HMENU              menu,
                                    const value_type&  callback) const;
    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        window_class m_class{};
    };

    inline window_group_class::window_group_class(const HINSTANCE hInstance, const string_view& classname, const UINT style) : m_class(hInstance, classname, style, &window_group_class::WindowProc)
    {
    }

    inline window_group_class::window_group_class(window_group_class&& other) noexcept : m_class(std::move(other.m_class))
    {
    }

    inline window_group_class& window_group_class::operator=(window_group_class&& other) noexcept
    {
        return *this;
    }

    inline bool window_group_class::registered() const noexcept
    {
        return m_class.registered();
    }

    inline ATOM window_group_class::value() const noexcept
    {
        return m_class.value();
    }

    inline window_handle window_group_class::create_window(const DWORD        extendedStyle,
                                                           const string_view& windowName,
                                                           const DWORD        style,
                                                           const int          x,
                                                           const int          y,
                                                           const int          width,
                                                           const int          height,
                                                           HWND               parent,
                                                           HMENU              menu,
                                                           const value_type&  callback) const
    {
        const auto proc_pointer = new value_type(callback);
        auto       proc_param   = window_handle::param_pointer(proc_pointer, [](auto ptr)
        {
            delete static_cast<pointer>(ptr);
        });

        auto result = window_handle(extendedStyle,
                                    m_class.name(),
                                    windowName,
                                    style,
                                    x,
                                    y,
                                    width,
                                    height,
                                    parent,
                                    menu,
                                    m_class.instance(),
                                    std::move(proc_param));
        return result;
    }

    inline LRESULT window_group_class::WindowProc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
    {
        switch (msg)
        {
        case WM_NCCREATE:
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
            SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
            break;
        case WM_DESTROY:
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
            break;
        default:
            {
                const auto proc = reinterpret_cast<pointer>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                if (proc)
                {
                    if ((*proc)(hwnd, msg, wParam, lParam))
                    {
                        return 0;
                    }
                }
            }
            break;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
