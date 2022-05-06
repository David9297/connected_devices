using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotator2 : MonoBehaviour
{
    public OSC osc;

    float horizontalSpeed = 0.000001f;
    float verticalSpeed = 0.000001f;

    float joystickX, joystickY;

	void Start ()
	{
    joystickX = Input.GetAxis("Mouse X");
    joystickY = Input.GetAxis("Mouse Y");

		osc.SetAddressHandler("/publish/func", OnReceiveData);
	}

    void Update()
    {
        float h = horizontalSpeed;
        float v = verticalSpeed;

        transform.Rotate(joystickX, joystickY, 0);
    }

    void OnReceiveData(OscMessage message)
	{
		joystickX = message.GetInt(5);
    joystickY = message.GetInt(6);

		Debug.Log("Joystick Button X Move State: " + joystickX);
    Debug.Log("Joystick Button Y Move State: " + joystickY);
	}
}
