#version 120

uniform sampler2D texRGB;
uniform sampler2D texMask;

void main()
{
	vec2 uvs = vec2(gl_TexCoord[0].st);
	vec2 i_uvs = vec2(gl_TexCoord[0].s, 1-gl_TexCoord[0].t);

	gl_FragColor.rgb = texture2D(texRGB, uvs).xyz;
	gl_FragColor.a = texture2D(texMask, i_uvs).x;
}