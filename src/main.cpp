#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

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

    CCLabelBMFont* ensureHint(CCNode* host, CCLabelBMFont* label, std::string const& buttonName) {
        if (!host) return nullptr;

        auto win = CCDirector::sharedDirector()->getWinSize();
        std::string text = "Press again within 3s to confirm " + buttonName; // optimized

        if (!label || label->getParent() != host) {
            label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
            if (!label) return nullptr;
            host->addChild(label, 3);
        } else {
            label->setString(text.c_str());
        }

        label->stopAllActions();
        label->setScale(0.45f);
        label->setPosition({ win.width / 2.f, win.height * 0.78f });
        label->setOpacity(255);
        label->setVisible(true);

        label->runAction(CCSequence::create(
            CCDelayTime::create(2.4f),
            CCFadeOut::create(0.35f),
            CCHide::create(),
            nullptr
        ));

        return label;
    }

    bool shouldRunWithConfirm(
        CCNode* host,
        Clock::time_point& lastPress,
        CCLabelBMFont*& hintLabel,
        char const* settingKey,
        std::string const& buttonName
    ) {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return true;

        auto now = Clock::now();

        if (lastPress.time_since_epoch().count() != 0) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPress).count();
            if (ms <= kConfirmWindowMs) {
                lastPress = {};
                hideHint(hintLabel);
                return true;
            }
        }

        lastPress = now;
        hintLabel = ensureHint(host, hintLabel, buttonName);
        return false;
    }
}

class $modify(EditorUI) {
    struct Fields {
        Clock::time_point pasteStatePress;
        Clock::time_point pasteColorPress;
        CCLabelBMFont* pasteStateHint = nullptr;
        CCLabelBMFont* pasteColorHint = nullptr;
    };

    void onPasteState(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->pasteStatePress,
            m_fields->pasteStateHint,
            "Confirm_PasteState",
            "Paste State"
        )) return;

        this->EditorUI::onPasteState(sender);
    }

    void onPasteColor(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->pasteColorPress,
            m_fields->pasteColorHint,
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
        CCLabelBMFont* pasteHint = nullptr;
        CCLabelBMFont* addGroupHint = nullptr;
        CCLabelBMFont* removeFromGroupHint = nullptr;
    };

    void onPaste(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->pastePress,
            m_fields->pasteHint,
            "Confirm_PasteState2",
            "Paste"
        )) return;

        this->SetGroupIDLayer::onPaste(sender);
    }

    void onAddGroup(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->addGroupPress,
            m_fields->addGroupHint,
            "Confirm_AddGroup",
            "Add Group"
        )) return;

        this->SetGroupIDLayer::onAddGroup(sender);
    }

    void onRemoveFromGroup(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->removeFromGroup,
            m_fields->removeFromGroupHint,
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
        CCLabelBMFont* alignXHint = nullptr;
        CCLabelBMFont* alignYHint = nullptr;
        CCLabelBMFont* buildHelperHint = nullptr;
    };

    void onAlignX(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->alignXPress,
            m_fields->alignXHint,
            "Confirm_AlignX",
            "Align X"
        )) return;

        this->EditorPauseLayer::onAlignX(sender);
    }

    void onAlignY(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->alignYPress,
            m_fields->alignYHint,
            "Confirm_AlignY",
            "Align Y"
        )) return;

        this->EditorPauseLayer::onAlignY(sender);
    }

    void onBuildHelper(CCObject* sender) {
        if (!shouldRunWithConfirm(
            this,
            m_fields->buildHelperPress,
            m_fields->buildHelperHint,
            "Confirm_BuildHelper",
            "Build Helper"
        )) return;

        this->EditorPauseLayer::onBuildHelper(sender);
    }
};