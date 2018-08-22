#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include <tetra/ColoredLine.hpp>
#include <tetra/Surface.hpp>
#include <tetra/gl/Framebuffer.hpp>
#include <tetra/gl/Texture.hpp>
#include <tetra/sdl/SdlEventPump.hpp>

namespace tetra
{
class PingPong final
{
  public:
    PingPong();
    PingPong(PingPong&& from) = default;
    PingPong(const PingPong& from) = delete;

    PingPong& operator=(PingPong&& from) = default;
    PingPong& operator=(const PingPong& from) = delete;

    /**
     * Apply some action with the target surface as the bound framebuffer
     * object.
     */
    template<typename RenderAction>
    void render(RenderAction action)
    {
        surface[target].render(action);
    }

    /**
     * Run a post-processing pass using the provided shader program.
     * Vertex Input:
     *   layout (location/0) in vec2 pos;
     *   layout (location=1) in vec2 uv;
     * Uniform Input:
     *   layout (location=0) uniform sampler2D current_frame;
     *
     * The program is used to process the current_frame sampler. The output is
     * saved to a separate buffer which becomes the current_frame once rendering
     * is complete (ping-pong).
     */
    void post_process_with(Program& program);

    /**
     * Blit the contents of the current frame onto the screen.
     * Assumes the screen size matches the last call to on_viewport_change.
     */
    void blit_to_screen() const;

    /**
     * Notify this resource that the viewport size has changed.
     */
    void on_viewport_change(int width, int height);

  private:
    class FullScreenQuad
    {
      public:
        FullScreenQuad();
        void draw();

      private:
        VAO vao;
        Buffer quad;
    } quad;

    Surface surface[2];
    int target = 0;
    int back = 1;
};

class GlApp final : public IWindowEvents
{
  public:
    GlApp();
    ~GlApp();

    virtual void on_frame_render() override;

    virtual void on_viewport_change(int width, int height) override;

  private:
    ColoredLine line;
    std::array<float, 16> view;

    Program gaussian_fade;
    PingPong ping_pong_surface;
};
} // namespace tetra

#endif
