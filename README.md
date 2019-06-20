TODO:
- targetColor / currentColor
  - dim brightness 5 when color changes
  - straight switch if MIN_BRIGHTNESS
- check battery effect
- get rid of strobe effect on left lying

## Flat
X:                     |=                    (0.04 g)
Y:                     |===========          (1.00 g)
Z:                     |===                  (0.25 g)

## Face Down
X:                     |==                   (0.13 g)
Y:                     |===                  (0.29 g)
Z:            =========|                     (-0.92 g)
Z < -0.82

## Face Up
X:                    =|                     (-0.20 g)
Y:                     |                     (-0.09 g)
Z:                     |==========           (0.98 g)
Z > 0.93

## Left
X:                     |==========           (0.97 g)
Y:                     |                     (-0.07 g)
Z:                     |=                    (0.01 g)
X > 0.9

## Right
X:           ==========|                     (-1.01 g)
Y:                     |=                    (0.09 g)
Z:                     |                     (-0.05 g)
X < -0.9


# Gravity Measuring
```c
void loop()
{
  // accel.available() will return 1 if new data is available, 0 otherwise
  if (accel.available())
  {
    // To update acceleration values from the accelerometer, call accel.read();
    accel.read();

    // After reading, six class variables are updated: x, y, z, cx, cy, and cz.
    // Those are the raw, 12-bit values (x, y, and z) and the calculated
    // acceleration's in units of g (cx, cy, and cz).

    // use the printAccelGraph funciton to print the values along with a bar
    // graph, to see their relation to eachother:
    printAccelGraph(accel.cx, "X", 20, 2.0);
    printAccelGraph(accel.cy, "Y", 20, 2.0);
    printAccelGraph(accel.cz, "Z", 20, 2.0);
    Serial.println();
  }

  // No need to delay, since our ODR is set to 1Hz, accel.available() will only return 1
  // about once per second.
}

// printAccelGraph prints a simple ASCII bar graph for a single accelerometer axis value.
// Examples:
//	printAccelGraph(-0.1, "X", 20, 2.0) will print:
// 		X:                    =|                     (0.1 g)
//	printAccelGraph(1.0, "Z", 20, 2.0) will print:
//		Z:                     |==========           (1.0 g)
// Input:
//	- [value]: calculated value of an accelerometer axis (e.g accel.cx, accel.cy)
//	- [name]: name of the axis (e.g. "X", "Y", "Z")
//	- [numBarsFull]: Maximum number of bars either right or left of 0 point.
//	- [rangeAbs]: Absolute value of the maximum acceleration range
void printAccelGraph(float value, String name, int numBarsFull, float rangeAbs)
{
  // Calculate the number of bars to fill, ignoring the sign of numBars for now.
  int numBars = abs(value / (rangeAbs / numBarsFull));

  Serial.print(name + ": "); // Print the axis name and a colon:

  // Do the negative half of the graph first:
  for (int i=0; i<numBarsFull; i++)
  {
    if (value < 0) // If the value is negative
    {
      // If our position in the graph is in the range we want to graph
      if (i >= (numBarsFull - numBars))
      Serial.print('='); // Print an '='
      else
      Serial.print(' '); // print spaces otherwise
    }
    else // If our value is positive, just print spaces
    Serial.print(' ');
  }

  Serial.print('|'); // Print a pipe (|) to represent the 0-point

  // Do the positive half of the graph last:
  for (int i=0; i<numBarsFull; i++)
  {
    if (value > 0)
    {	// If our position in the graph is in the range we want to graph
      if (i <= numBars)
      Serial.print('='); // Print an '='
      else
      Serial.print(' '); // otherwise print spaces
    }
    else // If value is negative, just print spaces
    Serial.print(' ');
  }

  // To end the line, print the actual value:
  Serial.println(" (" + String(value, 2) + " g)");
}
```
