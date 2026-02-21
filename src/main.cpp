#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include <chrono>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock; // timer

    constexpr int kHintPasteState   = 69001; // numbers for timers
    constexpr int kHintPasteState2  = 69002;
    constexpr int kHintAddGroup     = 69003;
    constexpr int kHintAlignX       = 69004;
    constexpr int kHintAlignY       = 69005;
    constexpr int kHintBuildHelper  = 69006;

    void removeHint(CCNode* host, int tag) {
        if (!host) return;
        host->removeChildByTag(tag, true);
    }

    void showHint(CCNode* host, int tag, char const* text) {
        if (!host) return;

        auto win = CCDirector::sharedDirector()->getWinSize();

        if (auto node = host->getChildByTag(tag)) {
            node->stopAllActions();
            if (auto label = typeinfo_cast<CCLabelBMFont*>(node)) {
                label->setOpacity(255);
            }
        } else {
            auto label = CCLabelBMFont::create(text, "bigFont.fnt");
            label->setTag(tag);
            label->setScale(0.45f);
            label->setOpacity(255);
            label->setPosition({ win.width / 2.f, win.height * 0.78f });
            host->addChild(label, 999999);
        }

        auto node = host->getChildByTag(tag);
        auto label = typeinfo_cast<CCLabelBMFont*>(node);
        if (!label) return;

        label->runAction(CCSequence::create(
            CCDelayTime::create(2.4f),
            CCFadeOut::create(0.35f),
            CCRemoveSelf::create(true),
            nullptr
        ));
    }

    bool shouldRunWithConfirm(CCNode* host, Clock::time_point& lastPress, int hintTag, char const* settingKey, char const* hintText) {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return true;

        auto now = Clock::now();

        if (lastPress.time_since_epoch().count() != 0) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPress).count();
            if (ms <= 3000) {
                lastPress = {};
                removeHint(host, hintTag);
                return true;
            }
        }

        lastPress = now;
        showHint(host, hintTag, hintText);
        return false;
    }
}

class $modify(EditorUI) {
    struct Fields {
        Clock::time_point pasteStatePress;
    };

    void onPasteState(CCObject* sender) {
        if (!shouldRunWithConfirm(
                this,
                m_fields->pasteStatePress,
                kHintPasteState,
                "Confirm_PasteState",
                "Press again within 3s to confirm PasteState"
            )) return;

        this->EditorUI::onPasteState(sender);
    }
};

class $modify(SetGroupIDLayer) {
    struct Fields {
        Clock::time_point pastePress;
        Clock::time_point addGroupPress;
    };

    void onPaste(CCObject* sender) {
        if (!shouldRunWithConfirm(
                this,
                m_fields->pastePress,
                kHintPasteState2,
                "Confirm_PasteState2",
                "Press again within 3s to confirm Paste"
            )) return;

        this->SetGroupIDLayer::onPaste(sender);
    }

    void onAddGroup(CCObject* sender) {
        if (!shouldRunWithConfirm(
                this,
                m_fields->addGroupPress,
                kHintAddGroup,
                "Confirm_AddGroup",
                "Press again within 3s to confirm AddGroup"
            )) return;

        this->SetGroupIDLayer::onAddGroup(sender);
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
                this,
                m_fields->alignXPress,
                kHintAlignX,
                "Confirm_AlignX",
                "Press again within 3s to confirm AlignX"
            )) return;

        this->EditorPauseLayer::onAlignX(sender);
    }

    void onAlignY(CCObject* sender) {
        if (!shouldRunWithConfirm(
                this,
                m_fields->alignYPress,
                kHintAlignY,
                "Confirm_AlignY",
                "Press again within 3s to confirm AlignY"
            )) return;

        this->EditorPauseLayer::onAlignY(sender);
    }

    void onBuildHelper(CCObject* sender) {
        if (!shouldRunWithConfirm(
                this,
                m_fields->buildHelperPress,
                kHintBuildHelper,
                "Confirm_BuildHelper",
                "Press again within 3s to confirm BuildHelper"
            )) return;

        this->EditorPauseLayer::onBuildHelper(sender);
    }
};