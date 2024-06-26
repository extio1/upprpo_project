#ifndef TEXT_EDITOR_BUTTON_H_
#define TEXT_EDITOR_BUTTON_H_

#include <string>

#include "glyph.h"

class Button : public Glyph {
   public:
    /**
     * @brief           Creates a button with the specified parameters and name.
     * @param x         Horizontal coordinate.
     * @param x         Vertical coordinate.
     * @param width     Button width.
     * @param height    Button height.
     * @param name      The name of the button can be displayed on the button
     * itself or used for identification.
     */
    explicit Button(const int x, const int y, const int width, const int height,
                    const std::string& name);

    Glyph::GlyphList Select(const Glyph::GlyphPtr& area) override { return Glyph::GlyphList(); }

    void Insert(GlyphPtr& glyph) override {}
    void Remove(const GlyphPtr& glyph) override {}
    void Add(GlyphPtr) override {}

    GlyphPtr GetFirstGlyph() override;
    GlyphPtr GetLastGlyph() override;
    GlyphPtr GetNextGlyph(GlyphPtr& glyph) override;
    GlyphPtr GetPreviousGlyph(GlyphPtr& glyph) override;

    /**
     * @brief           Returns whether the button is pressed.
     */
    bool IsPressed() const;

    std::shared_ptr<Glyph> Clone() const override;

   private:
    std::string name;
    bool isPressed = false;
};

#endif  // TEXT_EDITOR_BUTTON_H_