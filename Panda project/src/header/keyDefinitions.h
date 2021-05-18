#pragma once

//W-key
framework.define_key("w", "W-key", game::key_down, (void*)&game::w);
framework.define_key("shift-w", "W-key", game::key_down, (void*)&game::w);
framework.define_key("w-up", "W-key", game::key_up, (void*)&game::w);
framework.define_key("shift-w-up", "W-key", game::key_up, (void*)&game::w);

//A-key
framework.define_key("a", "A-key", game::key_down, (void*)&game::a);
framework.define_key("shift-a", "A-key", game::key_down, (void*)&game::a);
framework.define_key("a-up", "A-key", game::key_up, (void*)&game::a);
framework.define_key("shift-a-up", "A-key", game::key_up, (void*)&game::a);

//S-key
framework.define_key("s", "S-key", game::key_down, (void*)&game::s);
framework.define_key("shift-s", "S-key", game::key_down, (void*)&game::s);
framework.define_key("s-up", "S-key", game::key_up, (void*)&game::s);
framework.define_key("shift-s-up", "S-key", game::key_up, (void*)&game::s);

//D-key
framework.define_key("d", "D-key", game::key_down, (void*)&game::d);
framework.define_key("shift-d", "D-key", game::key_down, (void*)&game::d);
framework.define_key("d-up", "D-key", game::key_up, (void*)&game::d);
framework.define_key("shift-d-up", "D-key", game::key_up, (void*)&game::d);

//Space-key
framework.define_key("space", "Space-key", game::key_down, (void*)&game::space);
framework.define_key("shift-space", "Space-key", game::key_down, (void*)&game::space);
framework.define_key("space-up", "Space-key", game::key_up, (void*)&game::space);
framework.define_key("shift-space-up", "Space-key", game::key_up, (void*)&game::space);

//LShift-key
framework.define_key("lshift", "LShift-key", game::key_down, (void*)&game::lshift);
framework.define_key("lshift-up", "LShift-key", game::key_up, (void*)&game::lshift);

//Arrow keys
framework.define_key("arrow_up", "Arrow up-key", game::key_down, (void*)&game::arrow_up);
framework.define_key("arrow_up-up", "Arrow up-key", game::key_up, (void*)&game::arrow_up);

framework.define_key("arrow_down", "Arrow down-key", game::key_down, (void*)&game::arrow_down);
framework.define_key("arrow_down-up", "Arrow down-key", game::key_up, (void*)&game::arrow_down);

framework.define_key("arrow_left", "Arrow left-key", game::key_down, (void*)&game::arrow_left);
framework.define_key("arrow_left-up", "Arrow left-key", game::key_up, (void*)&game::arrow_left);

framework.define_key("arrow_right", "Arrow right-key", game::key_down, (void*)&game::arrow_right);
framework.define_key("arrow_right-up", "Arrow right-key", game::key_up, (void*)&game::arrow_right);

//R-key
//framework.define_key("r", "R-key", game::runPyScript, 0);
framework.define_key("r", "R-key", game::key_down, (void*)&game::r);
framework.define_key("r-up", "R-key", game::key_up, (void*)&game::r);

//Q-key
framework.define_key("q", "Q-key", game::key_down, (void*)&game::q);
framework.define_key("q-up", "Q-key", game::key_up, (void*)&game::q);

//E-key
framework.define_key("e", "E-key", game::key_down, (void*)&game::e);
framework.define_key("e-up", "E-key", game::key_up, (void*)&game::e);

//V-key
framework.define_key("v", "V-key", game::key_down, (void*)&game::v);
framework.define_key("v-up", "V-key", game::key_up, (void*)&game::v);

//F2-key
framework.define_key("f2", "f2-key", game::key_down, (void*)&game::f2);
framework.define_key("f2-up", "f2-key", game::key_up, (void*)&game::f2);

//F3-key
framework.define_key("f3", "f3-key", game::key_down, (void*)&game::f3);
framework.define_key("f3-up", "f3-key", game::key_up, (void*)&game::f3);

//F4-key
framework.define_key("f4", "f4-key", game::key_down, (void*)&game::f4);
framework.define_key("f4-up", "f4-key", game::key_up, (void*)&game::f4);

//F5-key
framework.define_key("f5", "f5-key", game::key_swap, (void*)&game::f5);

//F6-key
framework.define_key("f6", "f6-key", game::key_down, (void*)&game::f6);
framework.define_key("f6-up", "f6-key", game::key_up, (void*)&game::f6);

//Esc-key
framework.define_key("escape", "Esc-key", game::key_swap, (void*)&game::esc);

//Mouse button 1
framework.define_key("mouse1", "Mouse1-button", game::key_down, (void*)&game::mouse1);
framework.define_key("shift-mouse1", "Mouse1-button", game::key_down, (void*)&game::mouse1);

//Mouse button 2
framework.define_key("mouse2", "Mouse2-button", game::key_down, (void*)&game::mouse2);
framework.define_key("shift-mouse2", "Mouse2-button", game::key_down, (void*)&game::mouse2);

//Mouse button 3
framework.define_key("mouse3", "Mouse3-button", game::key_down, (void*)&game::mouse3);
framework.define_key("shift-mouse3", "Mouse3-button", game::key_down, (void*)&game::mouse3);

//Mouse wheel up
framework.define_key("wheel_up", "Wheel rolled upwards", game::wheel_roll, (void*)&game::wheel_up);
framework.define_key("shift-wheel_up", "Wheel rolled upwards", game::wheel_roll, (void*)&game::wheel_up);

//Mouse wheel down
framework.define_key("wheel_down", "Wheel rolled downwards", game::wheel_roll, (void*)&game::wheel_down);
framework.define_key("shift-wheel_down", "Wheel rolled downwards", game::wheel_roll, (void*)&game::wheel_down);

//Number keys
framework.define_key("1", "1-key", game::set_hotbar_slot, (void*)&game::num0);
framework.define_key("2", "2-key", game::set_hotbar_slot, (void*)&game::num1);
framework.define_key("3", "3-key", game::set_hotbar_slot, (void*)&game::num2);
framework.define_key("4", "4-key", game::set_hotbar_slot, (void*)&game::num3);
framework.define_key("5", "5-key", game::set_hotbar_slot, (void*)&game::num4);
framework.define_key("6", "6-key", game::set_hotbar_slot, (void*)&game::num5);
framework.define_key("7", "7-key", game::set_hotbar_slot, (void*)&game::num6);
framework.define_key("8", "8-key", game::set_hotbar_slot, (void*)&game::num7);
framework.define_key("9", "9-key", game::set_hotbar_slot, (void*)&game::num8);