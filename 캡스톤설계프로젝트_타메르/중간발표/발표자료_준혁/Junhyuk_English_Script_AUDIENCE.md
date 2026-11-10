# Junhyuk — midterm presentation script

Target: about 6 minutes for slides 7–10. The professor's outline and the rest of the team presentation come before this section.

## Slide 7 — Project implementation details: system flow

Now I’ll focus on the laptop software and how it connects what the rover sees to what the rover does.

At this stage, the operator drives the rover by holding a forward or reverse key. The Pi camera streams video to the laptop, where YOLO looks for a person. A separate safety controller decides whether driving is still allowed.

The laptop sends a drive or stop command to the Pi over Wi-Fi. The Pi sends status back; we call this telemetry. It shows the control and safety state so the operator knows why motion is allowed or blocked. It does not measure actual wheel speed.

There are two stop layers. The laptop blocks driving after a detection or stale video. The Pi also stops the motor if commands stop arriving. So a laptop or network failure cannot leave the rover driving indefinitely.

## Slide 8 — Project implementation details: detection and control

Here you can see the two code responsibilities. The detector returns a person box and confidence score, and draws the box on the video. That score indicates a possible person, not whether they are injured.

The safety controller receives that cue. It disarms movement, clears the held direction, and enters a stop state. Pressing forward again will not silently restart the rover. The operator must release the key and explicitly reset control after a fresh frame no longer shows the person.

We use a pretrained YOLO model. Our contribution is the connection from visual evidence to safe rover behavior. The model never directly controls the motor; it can only block motion through the safety controller. That separation also makes later perception features easier to add.

## Slide 9 — Project discussion: real rover test

We tested this loop on the physical rover on November 3. In the application image, you can see the person box and the stop state together. We recorded forward and reverse tests. In both, the operator started motion, the camera detected a person, and the rover stopped.

This demonstrates the complete path: Pi camera to laptop perception, laptop decision back to Pi, and real motor control. The network, application, and motor operated together on the rover.

The videos confirm stopping in these trials, but we have not measured stop time or distance. Next we will measure those and test different lighting and viewing angles. We also need to place a detection on a map. Right now, the camera tells us what is visible, but not where it is in the building.

## Slide 10 — Current status and next steps

Here is the current boundary: live video, person detection, manual driving with a safety stop, Pi motor commands, and returned status work together. That is what we can demonstrate today.

Next we want to make it a search mission. LiDAR and wheel odometry will help build a map and estimate the rover’s location. A mission layer will choose an unexplored area. Navigation tools such as ROS 2 Nav2 will plan and follow a route while avoiding obstacles. The Pi-side safety stop will still override movement.

When the camera finds a person candidate, the rover should stop or approach safely, get another view, and report an image and map location. Tracking or pose analysis may help us check the candidate. A person box alone cannot establish injury or identity. A higher-level AI model may choose among safe search goals, but it will not directly control the motors.

Our next milestones are mapping, navigation, detection verification, and a full search-and-report test. The forward and reverse stop clips in our demonstration show the working foundation for that sequence.
