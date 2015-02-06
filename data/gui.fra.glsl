#version 100

precision mediump float;

uniform sampler2D uTexture;

varying vec2 vTex;
varying vec4 vColor;

void main() {
	gl_FragColor = texture2D(uTexture, vTex) * vColor;
}

