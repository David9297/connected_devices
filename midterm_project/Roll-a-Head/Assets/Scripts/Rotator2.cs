using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotator2 : MonoBehaviour
{
    float horizontalSpeed = 2.0f;
    float verticalSpeed = 2.0f;

    // Update is called once per frame
    void Update()
    {
        // Get the mouse delta. This is not in the range -1...1
        float h = horizontalSpeed * Input.GetAxis("Mouse X");
        float v = verticalSpeed * Input.GetAxis("Mouse Y");

        transform.Rotate(v, h, 0);
    }
}
