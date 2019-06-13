#ifndef ACTION_H
#define ACTION_H

double trackbarValue = 0.0;

int calc(int a, int b) {
	return a + b;
}

void changeTrackBarPlus2() {
	trackbarValue += 2;
}
void changeFrame(int n) {
	trackbarValue += n;
}

#endif ACTION_H