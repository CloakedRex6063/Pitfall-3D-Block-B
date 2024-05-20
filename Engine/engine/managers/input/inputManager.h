#pragma once
#include "functional"
#include "string"
#include "unordered_map"
#include "vector"

enum Keys
{
    // Letters
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
    KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

    // Numbers
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

    // Special keys
    KEY_Up, KEY_Down, KEY_Left, KEY_Right, KEY_Enter, KEY_Escape, KEY_Backspace, KEY_Tab,
    KEY_Shift_Left, KEY_Shift_Right, KEY_Ctrl_Left, KEY_Ctrl_Right, KEY_Alt_Left, KEY_Alt_Right,
    KEY_Space, KEY_Num0, KEY_Num1, KEY_Num2, KEY_Num3, KEY_Num4, KEY_Num5, KEY_Num6, KEY_Num7,
    KEY_Num8, KEY_Num9, KEY_None,

    // Mouse buttons
    KEY_MouseLeft, KEY_MouseRight, KEY_MouseMiddle, KEY_Mouse4, KEY_Mouse5
};

enum InputEvent
{
    IE_Axis,
	IE_Pressed,
	IE_Held,
	IE_Released
};

enum InputType
{
	IT_Button,
	IT_Axis
	//Axis2D: Implement Later
};

enum InputModifier
{
    IM_None,
	IM_Negate
    //SwapAxis: Implement Later
};

struct InputAction
{
    std::unordered_map<Keys,InputModifier> keys;
    InputType type;
    InputAction(InputType type) : type(type) {}
};

struct InputBinding
{
    std::string name;
    InputEvent event;
    void* value;
    std::function<void(void*)> pressedCallback;
    std::function<void(void*)> heldCallback;
    std::function<void(void*)> releasedCallback;
    std::function<void(void*)> axisCallback;
};

class InputManager
{
public:
    void Tick();

    /**
     * \brief Adds an input action to the input manager
     * \param name Name of the input action
     * \param action Input action to add
     */
    void AddAction(const std::string& name, const InputAction& action);

    /**
     * \brief Binds an input action to a callback
     * \param name Name of the input action
     * \param event Event to bind the callback to
     * \param callback The callback to bind
     */
    void BindAction(const std::string& name, InputEvent event, const std::function<void(void*)>& callback);

    /**
     * \brief Binds an input axis to a callback
     * \param name Name of the input axis
     * \param callback The callback to bind
     */
	void BindAxis(const std::string& name, const std::function<void(void*)>& callback);

    void ClearBinding(const std::string& name, InputEvent event);

    /**
     * \brief Gets the value of an input action
     * \param name The name of the input action
     * \return float for axis
     */
    void* GetInputValue(const std::string& name) const;

    std::unordered_map<Keys,bool> keyStates;

private:

	/**
     * \brief Checks if the input action is pressed that frame
     * \param name Name of the input action
     * \param modifier Modifier to apply to the input action
     * \return True if the input action is pressed that frame, false otherwise
     */
    bool IsPressed(const std::string& name, InputModifier& modifier);

    /**
     * \brief Checks if the input action is held
     * \param name Name of the input action
     * \param modifier Modifier to apply to the input action
     * \return True if the input action is held, false otherwise
     */
    bool IsHeld(const std::string& name, InputModifier& modifier);

    /**
     * \brief Checks if the input action is released that frame
     * \param name Name of the input action
     * \param modifier Modifier to apply to the input action
     * \return True if the input action is released that frame, false otherwise
     */
    bool IsReleased(const std::string& name, InputModifier& modifier);

    std::unordered_map<std::string, InputAction> actions;
    std::vector<InputBinding> bindings;
};
