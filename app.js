'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Creating C Library


var libsvg = ffi.Library('./libsvgparse', {
  "testFunction": ['string', []],
  "createValidSVGandJSON": ['string', ['string', 'string']],
  "createValidSVGandComponentJSON": ['string', ['string', 'string']],
  "updateTitleOfImage": ['string', ['string', 'string', 'string', 'string']],
  "updateAttributeAndSaveFile": ['int', ['string', 'string', 'string', 'string', 'int', 'int']],
  "addRectangleToSVGandSave": ['int', ['string', 'string', 'float', 'float', 'float', 'float', 'string']],
  "addCircleToSVGandSave": ['int', ['string', 'string', 'float', 'float', 'float', 'string']]
});





async function dbConnect(username, password, database) {
    // get the client
    
    
  }



function isEmpty(obj) { 
  for (var key in obj) { 
      if (obj.hasOwnProperty(key)) { 
          return false; 
      } 
  }
  return true; 
}

//Sample endpoint
app.get('/update', function(req , res){
  let retObj = [];

  fs.readdir('./uploads/', (err, files) => {
    files.forEach(file => {
      if (err) {
        console.log(err);
      }
      let temp = JSON.parse(libsvg.createValidSVGandJSON(path.join(__dirname, '/uploads/', file), "parser/svg.xsd"));
      if (!isEmpty(temp)) {
        let fileStat = fs.statSync(path.join(__dirname, '/uploads/', file));
        let size = fileStat["size"];
        size = size / 1000;
        temp.filesize = size;
        temp.filename = file;
        retObj.push(temp);
      }
    });
    
    res.send(retObj);
  })
});



app.use('/editname', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/fillfiletable', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/asyncDB', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/initDB', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/editattribute', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/addrectangle', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/addcircle', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/newdownload', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/cleartables', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/updatechange', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/dbstatus', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/getfiletable', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/getcreateddates', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/getchangeddates', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.use('/getmostfreq', function( req, res, next ) {
  var data = '';
  req.on( 'data', function( chunk ) {
    data += chunk;
  });
  req.on( 'end', function() {
    req.body = data;
    next();
  });
});

app.post('/editname', function(req, res) {
  let temp = JSON.parse(req.body);
  let ret = libsvg.updateTitleOfImage(path.join(__dirname, '/uploads/', temp.file), temp.editTitle, temp.editDesc, "parser/svg.xsd");
  
  res.send("success");
});



app.post('/editattribute', function(req, res) {
  let temp = JSON.parse(req.body);
  let shape = temp.shape;
  let shapeArr = shape.split('_');
  var shapeType;
  if (shapeArr[0] == "rect") {
    shapeType = 2;
  }
  if (shapeArr[0] == "circle") {
    shapeType = 1;
  }
  if (shapeArr[0] == "path") {
    shapeType = 3;
  }
  if (shapeArr[0] == "group") {
    shapeType = 4;
  }
  var elemIndex = shapeArr[1] - 1;
  
  let temp2 = JSON.parse(libsvg.updateAttributeAndSaveFile(path.join(__dirname, '/uploads/', temp.file), "parser/svg.xsd", temp.name, temp.value, shapeType, elemIndex));
  res.status(200).send(JSON.stringify(temp2));
});

app.post('/addrectangle', function(req, res) {
  let temp = JSON.parse(req.body);
  let temp2 = JSON.parse(libsvg.addRectangleToSVGandSave(path.join(__dirname, '/uploads/', temp.file), "parser/svg.xsd", temp.x, temp.y, temp.width, temp.height, temp.units));
  res.status(200).send(JSON.stringify(temp2));
});

app.post('/addcircle', function(req, res) {
  let temp = JSON.parse(req.body);
  let temp2 = JSON.parse(libsvg.addCircleToSVGandSave(path.join(__dirname, '/uploads/', temp.file), "parser/svg.xsd", temp.cx, temp.cy, temp.r, temp.units));
  res.status(200).send(JSON.stringify(temp2));
});

app.get('/updatesvgpanel', function(req, res) {
  let newFile = req.query.file;
  let temp = JSON.parse(libsvg.createValidSVGandComponentJSON(path.join(__dirname, '/uploads/', newFile), "parser/svg.xsd"));
  
  res.send(temp);
});

app.post('/getfiletable', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  console.log(temp);

  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let getRowsFILE = "SELECT * FROM FILE";

    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);

    res.status(200).send(rowsA);
  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
}); 

app.post('/dbstatus', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  console.log(temp);

  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let getRowsFILE = "SELECT * FROM FILE";
    let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
    let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);
    const [rowsB, fieldsB] = await connection.execute(getRowsCHANGE);
    const [rowsC, fieldsC] = await connection.execute(getRowsDOWNLOAD);

    let data = {"N1": rowsA.length, "N2": rowsB.length, "N3": rowsC.length}

    res.status(200).send(data);
  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
}); 

app.post('/cleartables', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  console.log(temp);

  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;
  try {
    connection = await mysql.createConnection(dbConf);

    let sql = "DELETE FROM FILE";
    let sql2 = "DELETE FROM IMG_CHANGE";
    let sql3 = "DELETE FROM DOWNLOAD";

    await connection.execute(sql);
    await connection.execute(sql2);
    await connection.execute(sql3);

    let getRowsFILE = "SELECT * FROM FILE";
    let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
    let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);
    const [rowsB, fieldsB] = await connection.execute(getRowsCHANGE);
    const [rowsC, fieldsC] = await connection.execute(getRowsDOWNLOAD);

    let data = {"N1": rowsA.length, "N2": rowsB.length, "N3": rowsC.length}

    res.status(200).send(data);

  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});

app.post('/updatechange', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  console.log(temp);

  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let getId = "SELECT * FROM FILE WHERE file_name = '" + temp.filename + "' LIMIT 1";
    let [row, col] = await connection.execute(getId);
    let id = row[0].svg_id;
    console.log(id);
    

    let newChange = "INSERT INTO IMG_CHANGE (change_type, change_summary, change_time, svg_id) VALUES ('" + temp.type + "', '" + temp.summary + "', CURRENT_TIMESTAMP, '" + id + "')";
    console.log(newChange);
    await connection.execute(newChange);

    let getRowsFILE = "SELECT * FROM FILE";
    let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
    let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);
    const [rowsB, fieldsB] = await connection.execute(getRowsCHANGE);
    const [rowsC, fieldsC] = await connection.execute(getRowsDOWNLOAD);

    let data = {"N1": rowsA.length, "N2": rowsB.length, "N3": rowsC.length}

    res.status(200).send(data);
  } catch (error) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});

app.post('/asyncDB', async function(req, res, next) {

  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  

    let dbConf = {
      host     : 'dursley.socs.uoguelph.ca',
      user     : temp.username,
      password : temp.password,
      database : temp.database
    };
    
    let connection;

    try{
        // create the connection
        connection = await mysql.createConnection(dbConf);
        
        res.status(200).send("success");
        //Populate the table
        //Run select query, wait for results
    }catch(e){
        console.log("Query error: "+e);
        res.status(200).send("error");
    }finally {
        if (connection && connection.end) connection.end();
    }
  

});

app.post('/getchangeddates', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  const temp = JSON.parse(req.body);
  console.log(temp);
  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let sql = "SELECT * FROM IMG_CHANGE WHERE change_time BETWEEN '" + temp.date1 + "' AND '" + temp.date2 + "'";

    let [row, col] = await connection.execute(sql);
    let extra = [];
    let extra2 = [];
    
    for (let e of row) {
      let sql = "SELECT * FROM FILE WHERE svg_id = '" + e.svg_id + "' LIMIT 1";

      let [temprow,tempcol] = await connection.execute(sql);

      extra.push(e);
      extra2.push(temprow[0]);
      
    }

    let data = {'change': extra, 'file': extra2};

    
    res.status(200).send(data);
  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});

app.post('/getcreateddates', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  const temp = JSON.parse(req.body);
  
  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let sql = "SELECT * FROM FILE WHERE creation_time BETWEEN '" + temp.date1 + "' AND '" + temp.date2 + "'";
    

    let [row, col] = await connection.execute(sql);

    res.status(200).send(row);

  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});

app.post('/newdownload', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  const temp = JSON.parse(req.body);

  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp.username,
    password  : temp.password,
    database  : temp.database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let getFILE = "SELECT * FROM FILE WHERE file_name = '" + temp.file + "' LIMIT 1";

    let [row, col] = await connection.execute(getFILE);
    let address = req.connection.remoteAddress;
    let arr = address.split(':');
    
    

    let newDownload = "INSERT INTO DOWNLOAD (d_descr, svg_id) VALUES ('" + arr[3] + "', '" + row[0].svg_id + "')";
    await connection.execute(newDownload);

    let getRowsFILE = "SELECT * FROM FILE";
    let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
    let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);
    const [rowsB, fieldsB] = await connection.execute(getRowsCHANGE);
    const [rowsC, fieldsC] = await connection.execute(getRowsDOWNLOAD);

    let data = {"N1": rowsA.length, "N2": rowsB.length, "N3": rowsC.length}

    res.status(200).send(data);

  } catch (e) {
    console.log("Query error: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});

app.post('/fillfiletable', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);
  
  let dbConf = {
    host      : 'dursley.socs.uoguelph.ca',
    user      : temp[0].username,
    password  : temp[0].password,
    database  : temp[0].database
  }

  let connection;

  try {
    connection = await mysql.createConnection(dbConf);

    let imgs = temp[1];
    
    for (let elem of imgs) {
      let details = JSON.parse(libsvg.createValidSVGandComponentJSON(path.join(__dirname, '/uploads/', elem.filename), "parser/svg.xsd"));
      
      let checkStatement = "SELECT * FROM FILE WHERE file_name = '" + elem.filename + "'";
      let [rows0, fields0] = await connection.execute(checkStatement);

      let filename = elem.filename;
      let filesize = elem.filesize;
      let nrect = elem.numRect;
      let ncirc = elem.numCirc;
      let npath = elem.numPaths;
      let ngroup = elem.numGroups;
      let filetitle = details.title;
      let filedesc = details.description;

      if (filetitle === '') {
        filetitle = null;
      }
      if (filedesc === '') {
        filedesc = null;
      }


      if (rows0.length == 0) {
        if (filetitle !== null && filedesc !== null) {
          let ins = "INSERT INTO FILE (file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size) VALUES ('" + filename + "','" + filetitle + "','" + filedesc + "','" + nrect + "','" + ncirc + "','" + npath + "','" + ngroup + "', CURRENT_TIMESTAMP, '" + filesize + "')";
          
          await connection.execute(ins);
        }
        else if (filetitle === null && filedesc !== null) {
          let ins = "INSERT INTO FILE (file_name, file_description, n_rect, n_circ, n_path, n_group, creation_time, file_size) VALUES ('" + filename + "','" + filedesc + "','" + nrect + "','" + ncirc + "','" + npath + "','" + ngroup + "', CURRENT_TIMESTAMP, '" + filesize + "')";
          
          await connection.execute(ins);
        }
        else if (filetitle !== null && filedesc === null) {
          let ins = "INSERT INTO FILE (file_name, file_title, n_rect, n_circ, n_path, n_group, creation_time, file_size) VALUES ('" + filename + "','" + filetitle + "','" + nrect + "','" + ncirc + "','" + npath + "','" + ngroup + "', CURRENT_TIMESTAMP, '" + filesize + "')";
          
          await connection.execute(ins);
        }
        else if (filetitle === null && filedesc === null) {
          let ins = "INSERT INTO FILE (file_name, n_rect, n_circ, n_path, n_group, creation_time, file_size) VALUES ('" + filename + "','" + nrect + "','" + ncirc + "','" + npath + "','" + ngroup + "', CURRENT_TIMESTAMP, '" + filesize + "')";
          
          await connection.execute(ins);
        }
      }
    }
    

    let getRowsFILE = "SELECT * FROM FILE";
    let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
    let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
    let [rowsA, fieldsA] = await connection.execute(getRowsFILE);
    const [rowsB, fieldsB] = await connection.execute(getRowsCHANGE);
    const [rowsC, fieldsC] = await connection.execute(getRowsDOWNLOAD);

    let data = {"N1": rowsA.length, "N2": rowsB.length, "N3": rowsC.length}

    res.status(200).send(data);
    
  } catch (e) {
    console.log("Query error: " + e)
  } finally {
    
    

    if (connection && connection.end) connection.end();
    
  }
});

app.post('/initDB', async function(req, res, next) {
  const mysql = require('mysql2/promise');
  let temp = JSON.parse(req.body);

  let dbConf = {
    host     : 'dursley.socs.uoguelph.ca',
    user     : temp.username,
    password : temp.password,
    database : temp.database
  };
  
  let connection;
  

  try{
      // create the connection
      connection = await mysql.createConnection(dbConf);

      let checkFILE = "SELECT * FROM information_schema.tables WHERE table_schema = '" + temp.database + "' AND table_name = '" + "FILE" + "' LIMIT 1";
      let checkCHANGE = "SELECT * FROM information_schema.tables WHERE table_schema = '" + temp.database + "' AND table_name = '" + "IMG_CHANGE" + "' LIMIT 1";
      let checkDOWNLOAD = "SELECT * FROM information_schema.tables WHERE table_schema = '" + temp.database + "'AND table_name = '" + "DOWNLOAD" + "' LIMIT 1";
      
      try {
        
        const [rows1, fields1] = await connection.execute(checkFILE);
        const [rows2, fields2] = await connection.execute(checkCHANGE);
        const [rows3, fields3] = await connection.execute(checkDOWNLOAD);
        if (rows1.length == 0) {
          let newFILE = "CREATE TABLE FILE (svg_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, file_title VARCHAR(256), file_description VARCHAR(256), n_rect INT NOT NULL, n_circ INT NOT NULL, n_path INT NOT NULL, n_group INT NOT NULL, creation_time DATETIME NOT NULL, file_size INT NOT NULL)";
          await connection.execute(newFILE);
        }
        else {
          console.log("FILE table already exists");
        }

        if (rows2.length == 0) {
          let newCHANGE = "CREATE TABLE IMG_CHANGE (change_id INT AUTO_INCREMENT PRIMARY KEY, change_type VARCHAR(256) NOT NULL, change_summary VARCHAR(256) NOT NULL, change_time DATETIME NOT NULL, svg_id INT NOT NULL, FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)";
          await connection.execute(newCHANGE);
          console.log("CHANGE table created");
        }
        else {
          console.log("CHANGE table already exists");
        }

        if (rows3.length == 0) {
          let newDOWNLOAD = "CREATE TABLE DOWNLOAD (download_id INT AUTO_INCREMENT PRIMARY KEY, d_descr VARCHAR(256), svg_id INT NOT NULL, FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)";
          await connection.execute(newDOWNLOAD);
          console.log("DOWNLOAD table created");
        }
        else {
          console.log("DOWNLOAD table already exists");
        }

        let getRowsFILE = "SELECT * FROM FILE";
        let getRowsCHANGE = "SELECT * FROM IMG_CHANGE";
        let getRowsDOWNLOAD = "SELECT * FROM DOWNLOAD";
        const [rows4, fields4] = await connection.execute(getRowsFILE);
        const [rows5, fields5] = await connection.execute(getRowsCHANGE);
        const [rows6, fields6] = await connection.execute(getRowsDOWNLOAD);

        let data = {"N1": rows4.length, "N2": rows5.length, "N3": rows6.length}

        res.status(200).send(data);

      } catch (e) {
        console.log(e);
      }
      
      
      //Populate the table
      //Run select query, wait for results
  }catch(e){
      console.log("Query error: "+e);
      res.status(400).send("error");
  }finally {
      if (connection && connection.end) connection.end();
  }
});



app.listen(portNum);
console.log('Running app at localhost: ' + portNum);