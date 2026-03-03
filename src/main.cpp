#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/ui/Notification.hpp>

#include <chrono>
#include <string>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock;

    constexpr int kConfirmWindowMs = 3000;

    void hideHint(CCLabelBMFont* label) {
        if (!label) return;
        label->stopAllActions();
        label->setOpacity(0);
        label->setVisible(false);
    }

    bool shouldRunWithConfirm(
        Clock::time_point& lastPress, // timer
        char const* settingKey, // literally key of setting
        std::string const& buttonName
    ) {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return true;

        // checking if "Confirm_EditSeveralObjs" is enabled
        if (Mod::get()->getSettingValue<bool>("Confirm_EditSeveralObjs")) {
            int count = 0;

            auto lel = LevelEditorLayer::get();
            if (lel && lel->m_editorUI && lel->m_editorUI->m_selectedObjects) {
                count = lel->m_editorUI->m_selectedObjects->count();
            }

            if (count <= 1)
                return true;
        }

        auto now = Clock::now();

        if (lastPress != Clock::time_point{}) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPress).count();
            if (ms <= kConfirmWindowMs) {
                lastPress = {};
                return true;
            }
        }

        lastPress = now;

        Notification::create(
            "Press again within 3s to confirm " + buttonName,
            NotificationIcon::Info,
            3.f
        )->show();

        return false;
    }
}

class $modify(EditorUI) {
    struct Fields {
        Clock::time_point pasteStatePress;
        Clock::time_point pasteColorPress;
    };

    void onPasteState(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->pasteStatePress,
            "Confirm_PasteState",
            "Paste State"
        )) return;

        this->EditorUI::onPasteState(sender);
    }

    void onPasteColor(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->pasteColorPress,
            "Confirm_PasteColor",
            "Paste Color"
        )) return;

        this->EditorUI::onPasteColor(sender);
    }
};

class $modify(SetGroupIDLayer) {
    struct Fields {
        Clock::time_point pastePress;
        Clock::time_point addGroupPress;
        Clock::time_point removeFromGroup;
    };

    void onPaste(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->pastePress,
            "Confirm_PasteState2",
            "Paste"
        )) return;

        this->SetGroupIDLayer::onPaste(sender);
    }

    void onAddGroup(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->addGroupPress,
            "Confirm_AddGroup",
            "Add Group"
        )) return;

        this->SetGroupIDLayer::onAddGroup(sender);
    }

    void onRemoveFromGroup(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->removeFromGroup,
            "Confirm_RemoveFromGroup",
            "Remove Group"
        )) return;

        this->SetGroupIDLayer::onRemoveFromGroup(sender);
    }
};

class $modify(EditorPauseLayer) {
    struct Fields {
        Clock::time_point alignXPress;
        Clock::time_point alignYPress;
        Clock::time_point buildHelperPress;
    };

    void onAlignX(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->alignXPress,
            "Confirm_AlignX",
            "Align X"
        )) return;

        this->EditorPauseLayer::onAlignX(sender);
    }

    void onAlignY(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->alignYPress,
            "Confirm_AlignY",
            "Align Y"
        )) return;

        this->EditorPauseLayer::onAlignY(sender);
    }

    void onBuildHelper(CCObject* sender) {
        if (!shouldRunWithConfirm(
            m_fields->buildHelperPress,
            "Confirm_BuildHelper",
            "Build Helper"
        )) return;

        this->EditorPauseLayer::onBuildHelper(sender);
    }
};