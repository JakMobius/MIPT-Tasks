//
// Created by Артем on 09.11.2021.
//

#include "alert-window.hpp"

AlertWindow::AlertWindow(const char* text) : UIWindow({}, {420, 150}) {

    set_title("Alert");
    auto* alert_text = new UIText({100, 10}, {250, 110}, text);
    alert_text->get_text_drawer()->set_v_alignment(VTextAlignmentCenter);
    alert_text->get_text_drawer()->set_font_size(18);

    auto* alert_texture_view = new UIView({20, 35}, {60, 60});
    alert_texture_view->set_fill_style(alert_texture);

    auto* ok_button = new UIButton({340, 110}, {70, 30});
    ok_button->set_title("Ok");

    ok_button->set_callback([this]() {
        DispatchQueue::main.push(DispatchQueueTask {[this]() {
            close();
        }});
    });

    get_content_view()->append_child(alert_text);
    get_content_view()->append_child(alert_texture_view);
    get_content_view()->append_child(ok_button);
}
