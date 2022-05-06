using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.InputSystem;
using TMPro;

public class PlayerController : MonoBehaviour
{
	public OSC osc;

	public float speed;
	public TextMeshProUGUI countText;
	public GameObject winTextObject;

    private float movementX;
    private float movementY;

	private Rigidbody rb;
	private int count;

	int OSCButton, buttonOne, buttonTwo, buttonThree, buttonFour, joystickPress;

	void Start ()
	{
		rb = GetComponent<Rigidbody>();

		count = 0;

		SetCountText ();

        winTextObject.SetActive(false);

		osc.SetAddressHandler("/publish/func", OnReceiveData);
	}

	void FixedUpdate ()
	{
		Vector3 movement = new Vector3 (movementX, 0.0f, movementY);

		rb.AddForce (movement * speed);
	}

	void OnTriggerEnter(Collider other)
	{
		if (other.gameObject.CompareTag ("PickUp"))
		{
			other.gameObject.SetActive (false);

			count = count + 1;

		    SetCountText ();
		}
	}

    void OnMove(InputValue value)
    {
        Vector2 v = value.Get<Vector2>();

        movementX = v.x;
        movementY = v.y;
    }

    void SetCountText()
	{
		countText.text = "Eggs Collected: " + count.ToString();

		if (count >= 52)
		{
            winTextObject.SetActive(true);
		}
    }
	void Update()
	{
		if(buttonOne == 1)
		{
			movementY += 0.005f;
		}

		if (buttonTwo == 1)
		{
			movementX -= 0.005f;
		}

		if(buttonThree == 1)
		{
			movementY -= 0.005f;
		}

		if(buttonFour == 1)
		{
			movementX += 0.005f;
		}

		if(joystickPress == 1)
		{
      transform.Translate(Vector3.up * 0.15f, Space.World);
		}
	}

	void OnReceiveData(OscMessage message)
	{
		OSCButton = message.GetInt(0);
		buttonOne = message.GetInt(1);
		buttonTwo = message.GetInt(2);
		buttonThree = message.GetInt(3);
		buttonFour = message.GetInt(4);
		joystickPress = message.GetInt(7);

		Debug.Log("OSC Button State: " + OSCButton);
		Debug.Log("Up Button State: " + buttonOne);
		Debug.Log("Left Button State: " + buttonTwo);
		Debug.Log("Down Button State: " + buttonThree);
		Debug.Log("Right Button State: " + buttonFour);
		Debug.Log("Joystick Button State: " + joystickPress);
	}
}
