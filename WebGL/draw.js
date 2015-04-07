// =============================================================================
// gl api calls to draw in the canvas
// =============================================================================


// ================ Global Variables ======================
var gl;
var canvas;
var shaderProgram;

// =================== Shader Code =========================

var vertex_shader_source =
    "attribute vec3 aVertexPosition;"                                                       +
    "uniform mat4 uModelViewMatrix;"                                                        +
    "uniform mat4 uProjectionMatrix;"                                                       +

    "void main(void) {"                                                                     +
    "    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aVertexPosition, 1.0);"  +
    "}"

var fragment_shader_source =
    "void main(void) {"                                 +
        "gl_FragColor = vec4(0.3, 0.5, 0.1, 1.0);"      +
    "}"

// ========================================================


function CreateShaderProgram() 
{
    var vertexShader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShader, vertex_shader_source);
    gl.compileShader(vertexShader);

    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShader, fragment_shader_source);
    gl.compileShader(fragmentShader);
        
    shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    gl.useProgram(shaderProgram);
}


function DrawWebGL() 
{
    var viewportWidth = canvas.clientWidth;
    var viewportHeight = canvas.clientHeight;

    //gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // Set the perspective matrix
    var pMatrix = mat4.create();
    mat4.perspective(45, viewportWidth / viewportHeight, 0.1, 100.0, pMatrix);
    uProjectionMatrixLoc = gl.getUniformLocation(shaderProgram, "uProjectionMatrix");
    gl.uniformMatrix4fv(uProjectionMatrixLoc, false, pMatrix);

    // Initialize the ModelView matrix
    var mvMatrix = mat4.create();
    mat4.identity(mvMatrix);

    var triangleVertices = [ 0.0,  1.0, 0.0,
                            -1.0, -1.0, 0.0,
                             1.0, -1.0, 0.0 ];

    var triangleVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexPositionBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(triangleVertices), gl.STATIC_DRAW);

    mat4.translate(mvMatrix, [-1.5, 0.0, -7.0]);
    var uModelViewMatrixLoc = gl.getUniformLocation(shaderProgram, "uModelViewMatrix");
    gl.uniformMatrix4fv(uModelViewMatrixLoc, false, mvMatrix);

    var aVertexPositionLoc = gl.getAttribLocation(shaderProgram, "aVertexPosition");
    gl.enableVertexAttribArray(aVertexPositionLoc);
    gl.vertexAttribPointer(aVertexPositionLoc, 3, gl.FLOAT, false, 0, 0);
    gl.drawArrays(gl.TRIANGLES, 0, 3);

    rectangleVertices = [ 1.0, 1.0, 0.0,
                         -1.0, 1.0, 0.0,
                          1.0, -1.0, 0.0,
                         -1.0, -1.0, 0.0 ];

    var rectangleVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, rectangleVertexPositionBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(rectangleVertices), gl.STATIC_DRAW);

    mat4.translate(mvMatrix, [3.0, 0.0, 0.0]);
    gl.uniformMatrix4fv(uModelViewMatrixLoc, false, mvMatrix);

    gl.enableVertexAttribArray(aVertexPositionLoc);
    gl.vertexAttribPointer(aVertexPositionLoc, 3, gl.FLOAT, false, 0, 0);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
}

function CreateRenderingContext() {
    try {
        gl = canvas.getContext("experimental-webgl");
    }
    catch (e) { }

    if (!gl) {
        alert("Could not initialise WebGL!");
    }
}

function StartWebGL() 
{
    canvas = document.getElementById("canvas-id");

    CreateRenderingContext();
    CreateShaderProgram();
    DrawWebGL();
}


