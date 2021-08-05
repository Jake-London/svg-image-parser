// Put all onload AJAX calls here, and event listeners
function isObjEmpty(obj) {
    for (var key in obj) { 
        if (obj.hasOwnProperty(key)) { 
            return false; 
        } 
    }
    return true; 
}
function createAttributeTable(selected, file) {

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: 'updatesvgpanel?file=' + file,
        success: function(data) {

            let selected = $('#show-attributes-select').val();
            var arr = selected.split('_');
            $('#attribute-info-panel').children('.attribute').remove();
            $('#edit-attribute-select').children('option').remove();

            Object.keys(data).forEach(key => {
                if (key == arr[0]) {
                    let temp = data[key];
                    let attribute = temp[arr[1] - 1].attributes;
                    var num2 = 1;
                    if (!isObjEmpty(attribute[0])) {
                        attribute.forEach(attr => {
                            var newDiv = $('<div class="attribute attr-info-sub" id="attribute_' + num2 + '"></div>');
                            newDiv.append('<p>Attribute ' + num2 + '</p>');
                            newDiv.append('<p>' + attr.name + '</p>');
                            newDiv.append('<p>' + attr.value + '</p>');
                            newDiv.appendTo('#attribute-info-panel');

                            var newOption = $('<option value="attribute_' + num2 + '"></option>');
                            newOption.html("Attribute " + num2);
                            newOption.appendTo('#edit-attribute-select');
                            num2++;
                        });
                    }
                }
            });

        }
    })

}

function createTable(newFile) {
    console.log(newFile);

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/updatesvgpanel?file=' + newFile,
        success: function(data) {
            console.log(data);
            $('#focus-img').remove();
            $('#image-cont').append('<img src="' + newFile + '" alt="' + newFile + '" id="focus-img">');
            $('#panel-2').children('.component').remove();
            $('#show-attributes-select').children('option').remove();
            /* $('#svg-view-panel').append('<div id="panel-2"></div>'); */
            $('#title').html(data.title);
            $('#description').html(data.description);
            var num = 1;
            if (!isObjEmpty(data.rect[0])) {
                data.rect.forEach(elem => {
                    var newDiv = $('<div id="rect' + num + '" class="component"></div>');
                    newDiv.append('<p class="section-1">Rectangle ' + num + '</p>');
                    newDiv.append('<p class="section-2">Upper left corner: x = ' + elem.x + elem.units + ', y = ' + elem.y + elem.units + ', Width = ' + elem.w + elem.units + ', Height = ' + elem.h + elem.units + '</p>');
                    newDiv.append('<p class="section-3">' + elem.numAttr + '</p>');
                    newDiv.appendTo('#panel-2');
                    var newOption = $('<option value="rect_' + num + '"></option>');
                    newOption.html("Rectangle " + num);
                    newOption.appendTo('#show-attributes-select');
                    num++;
                });
            }
            var num = 1;
            if (!isObjEmpty(data.circle[0])) {
                data.circle.forEach(elem => {
                    
                    var newDiv = $('<div id=circle' + num + '" class="component"></div>');
                    newDiv.append('<p class="section-1">Circle ' + num + '</p>');
                    newDiv.append('<p class="section-2">Center: x = ' + elem.cx + elem.units + ', y = ' + elem.cy + elem.units + ', radius = ' + elem.r + elem.units + '</p>');
                    newDiv.append('<p class="section-3">' + elem.numAttr + '</p>');
                    newDiv.appendTo('#panel-2');
                    var newOption = $('<option value="circle_' + num + '"></option>');
                    newOption.html("Circle " + num);
                    newOption.appendTo('#show-attributes-select');
                    num++;
                });
            }
            var num = 1;
            if (!isObjEmpty(data.path[0])) {
                data.path.forEach(elem => {
                    var newDiv = $('<div id=path' + num + '" class="component"></div>');
                    newDiv.append('<p class="section-1">Path ' + num + '</p>');
                    newDiv.append('<p class="section-2">Path data = ' + elem.d + '</p>');
                    newDiv.append('<p class="section-3">' + elem.numAttr + '</p>');
                    newDiv.appendTo('#panel-2');
                    var newOption = $('<option value="path_' + num + '"></option>');
                    newOption.html("Path " + num);
                    newOption.appendTo('#show-attributes-select');
                    num++;
                });
            }
            var num = 1;
            if (!isObjEmpty(data.group[0])) {
                data.group.forEach(elem => {
                    var newDiv = $('<div id=group' + num + '" class="component"></div>');
                    newDiv.append('<p class="section-1">Group ' + num + '</p>');
                    newDiv.append('<p class="section-2">' + elem.children + ' child elements</p>');
                    newDiv.append('<p class="section-3">' + elem.numAttr + '</p>');
                    newDiv.appendTo('#panel-2');
                    var newOption = $('<option value="group_' + num + '"></option>');
                    newOption.html("Group " + num);
                    newOption.appendTo('#show-attributes-select');
                    num++;
                });
            }
            let selected = $('#show-attributes-select').val();
            var arr = selected.split('_');
            var num = 0;
            $('#attribute-info-panel').children('.attribute').remove();
            $('#edit-attribute-select').children('option').remove();

            Object.keys(data).forEach(key => {
                if (key == arr[0]) {
                    let temp = data[key];
                    let attribute = temp[arr[1] - 1].attributes;
                    var num2 = 1;
                    if (!isObjEmpty(attribute[0])) {
                        attribute.forEach(attr => {
                            var newDiv = $('<div class="attribute attr-info-sub" id="attribute_' + num2 + '"></div>');
                            newDiv.append('<p>Attribute ' + num2 + '</p>');
                            newDiv.append('<p>' + attr.name + '</p>');
                            newDiv.append('<p>' + attr.value + '</p>');
                            newDiv.appendTo('#attribute-info-panel');

                            var newOption = $('<option value="attribute_' + num2 + '"></option>');
                            newOption.html("Attribute " + num2);
                            newOption.appendTo('#edit-attribute-select');
                            num2++;
                        });
                    }
                }
            });
            
        },
        fail: function(error) {
            console.log(error);
        }
    })
} 

$(document).ready(function() {
    // On page-load AJAX Example

    if (sessionStorage.getItem('username') !== null) {
        var formData = '{"username":"' + sessionStorage.getItem('username') + '","password":"' + sessionStorage.getItem('password') + '","database":"' + sessionStorage.getItem('database') + '"}';

        console.log(formData);

        $.ajax({
            type: 'POST',
            url: '/asyncDB',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: formData,
            success: function(data) {
                

                if (data == "success") {
                    let temp = JSON.parse(formData);
                    sessionStorage.setItem('username', temp.username);
                    sessionStorage.setItem('password', temp.password);
                    sessionStorage.setItem('database', temp.database);
                    
                    $('#main-cont').removeAttr('hidden');
                    $('#login-cont').attr("hidden", "true");

                    
                }
            }
        });

        $.ajax({
            type: 'GET',
            url: '/update',
            datatype: 'json',
            success: function(data) {
                let formData = [];
                let login = {"username": sessionStorage.getItem('username'), "password": sessionStorage.getItem('password'), "database": sessionStorage.getItem('database')};
                formData.push(login);
                formData.push(data);
                console.log(formData);
                console.log("hello");
                if (data.length > 0) {
                    $.ajax({
                        type: 'POST',
                        url: '/fillfiletable',
                        cache: false,
                        processData: false,
                        contentType: 'text/plain',
                        data: JSON.stringify(formData),
                        success: function(data2) {
                            alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                        }
                    })
                }
            }
        })



        $.ajax({
            type: 'GET',
            url: '/update',
            dataType: 'json',
            success: function(data) {
                console.log(data.length);
                if (data.length > 0) {
                    $('#store-files-form').removeAttr('hidden');
                    $('#clear-tables-form').removeAttr('hidden');
                    $('#db-status-form').removeAttr('hidden');
                    
                }
            }

        })

        
    }

    $(document).on('click', '.download-link', function() {
        console.log($(this).text());

        let downloadedFile = $(this).text();
        let username = sessionStorage.getItem('username');
        let password = sessionStorage.getItem('password');
        let database = sessionStorage.getItem('database');

        let formData = {'file': downloadedFile, 'username': username, 'password': password, 'database': database}

        $.ajax({
            type: 'POST',
            url: '/newdownload',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: JSON.stringify(formData),
            success: function(data) {
                alert("Database has " + data.N1 + " files, " + data.N2 + " changes, and " + data.N3 + " downloads");
            }
        })
    })

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/update',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            
            console.log("HELLO TESTING");
            if (!isObjEmpty(data)) {
                data.forEach(element => {
                    var newRow = $('<tr class="new-row">HELLO</tr>');
                    newRow.append('<td><a href="' + element.filename + '"><img src="' + element.filename + '"></a></td>');
                    newRow.append('<td><a href="' + element.filename + '" class="download-link" download>' + element.filename + '</a></td>');
                    newRow.append('<td>' + Math.round(element.filesize) + ' KB</td>');
                    newRow.append('<td>' + element.numRect + '</td>');
                    newRow.append('<td>' + element.numCirc + '</td>');
                    newRow.append('<td>' + element.numPaths + '</td>');
                    newRow.append('<td>' + element.numGroups + '</td>');
                    newRow.appendTo("#file-log-panel");

                    var newOption = $('<option value="' + element.filename + '"></option');
                    newOption.html(element.filename);
                    newOption.appendTo('#view-panel-select');

                    var newOption2 = $('<option value="' + element.filename + '"></option');
                    newOption2.html(element.filename);
                    newOption2.appendTo('#add-file');

                    let selected = $('#view-panel-select').val();
                    createTable(selected);
                    //We write the object to the console to show that the request was successful
                    console.log(data);
                    
                });
            }

            

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });
    
    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#upload-form').submit(function(e){
        $('#submit-res').html("File was submitted");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        
        var formData = new FormData();
        formData.append('uploadFile', $('#uploadFile')[0].files[0]);
        
        $.ajax({
            //Create an object for connecting to another waypoint
            type:'POST',
            url:'/upload',
            cache: false,
            processData:false,
            contentType:false,
            data:formData,
            success: function(data) {
                alert("File successfully uploaded");
                location.reload();
                console.log("success");
            }
        });
    });

    $('#login-form').submit(function(e) {
        e.preventDefault();

        var username = $('#username').val();
        var password = $('#password').val();
        var database = $('#database').val();

        var formData = '{"username":"' + username + '","password":"' + password + '","database":"' + database + '"}';

        console.log(formData);

        $.ajax({
            type: 'POST',
            url: '/asyncDB',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: formData,
            success: function(data) {
                

                if (data == "success") {
                    let temp = JSON.parse(formData);
                    sessionStorage.setItem('username', temp.username);
                    sessionStorage.setItem('password', temp.password);
                    sessionStorage.setItem('database', temp.database);
                    
                    $('#main-cont').removeAttr('hidden');
                    $('#login-cont').attr("hidden", "true");

                    $.ajax({
                        type: 'POST',
                        url: '/initDB',
                        cache: false,
                        contentType: 'text/plain',
                        data: formData,
                        success: function(data) {
                            console.log(data);
                            alert("Database has " + data.N1 + " files, " + data.N2 + " changes, and " + data.N3 + " downloads");

                            $.ajax({
                                type: 'GET',
                                url: '/update',
                                dataType: 'json',
                                success: function(data) {
                                    console.log(data.length);
                                    if (data.length > 0) {
                                        $('#store-files-form').removeAttr('hidden');
                                        $('#clear-tables-form').removeAttr('hidden');
                                        $('#db-status-form').removeAttr('hidden');

                                    }
                                }

                            })
                        }
                    })
                }
                else {
                    alert("Error: incorrect login info. Please try again.");
                }
            }
        })
    });

    $('#store-files-form').submit(function(e) {
        e.preventDefault();

        $.ajax({
            type: 'GET',
            url: '/update',
            datatype: 'json',
            success: function(data) {
                let formData = [];
                let login = {"username": sessionStorage.getItem('username'), "password": sessionStorage.getItem('password'), "database": sessionStorage.getItem('database')};
                formData.push(login);
                formData.push(data);
                console.log(formData);
                console.log("hello");
                if (data.length > 0) {
                    $.ajax({
                        type: 'POST',
                        url: '/fillfiletable',
                        cache: false,
                        processData: false,
                        contentType: 'text/plain',
                        data: JSON.stringify(formData),
                        success: function(data2) {
                            alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                        }
                    })
                }
            }
        })
    });

    $('#clear-tables-form').submit(function(e) {
        e.preventDefault();

        let formData = {'username': sessionStorage.getItem('username'), 'password': sessionStorage.getItem('password'), 'database': sessionStorage.getItem('database')};

        $.ajax({
            type: 'POST',
            url: '/cleartables',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: JSON.stringify(formData),
            success: function(data2) {
                alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                location.reload();
            }
        })
    });

    $('#db-status-form').submit(function(e) {
        e.preventDefault();

        let formData = {'username': sessionStorage.getItem('username'), 'password': sessionStorage.getItem('password'), 'database': sessionStorage.getItem('database')};

        $.ajax({
            type: 'POST',
            url: '/dbstatus',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: JSON.stringify(formData),
            success: function(data2) {
                alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                location.reload();
            }
        })
    });

    $('#infoEdit').submit(function(e){
        e.preventDefault();

        var title = $('#editTitle').val();
        var desc = $('#editDesc').val();
        var file = $('#view-panel-select').val();
        var formData = '{"editTitle":"' + title + '","editDesc":"' + desc + '","file":"' + file + '"}';
        
        console.log(formData);
        $.ajax({
            type:'POST',
            url:'/editname',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data:formData,
            success: function(data) {
                console.log("success");
                let formData;

                formData = {'username': sessionStorage.getItem('username'),'password': sessionStorage.getItem('password') , 'database': sessionStorage.getItem('database') ,'type': 'updating title & description', 'filename': file, 'summary': 'new title/description added to ' + file + ' with values title: ' + title + ', description: ' + desc}
                console.log(formData);
                $.ajax({
                    type: 'POST',
                    url: '/updatechange',
                    cache: false,
                    processData: false,
                    contentType: 'text/plain',
                    data: JSON.stringify(formData),
                    success: function(data2) {
                        alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                        location.reload();
                    }
                });
                
            }
        })
    });

    $('#attrEdit').submit(function(e){
        e.preventDefault();

        var name = $('#attrName').val();
        var val = $('#attrVal').val();
        var selected = $('#edit-attribute-select').val();
        var shape = $('#show-attributes-select').val();
        var file = $('#view-panel-select').val();

        var formData = '{"name":"' + name + '","value":"' + val + '","Attrid":"' + selected + '","shape":"' + shape + '","file":"' + file + '"}';

        $.ajax({
            type:'POST',
            url:'/editattribute',
            cache:false,
            processData:false,
            contentType: 'text/plain',
            data:formData,
            success: function(data) {
                console.log(data.toString());
                if (data.toString() == 1) {
                    let formData;

                    formData = {'username': sessionStorage.getItem('username'),'password': sessionStorage.getItem('password') , 'database': sessionStorage.getItem('database') ,'type': 'editing attribute', 'filename': file, 'summary': 'changing/adding attribute to ' + file + ' with values name: ' + name + ', value: ' + val}
                    console.log(formData);
                    $.ajax({
                        type: 'POST',
                        url: '/updatechange',
                        cache: false,
                        processData: false,
                        contentType: 'text/plain',
                        data: JSON.stringify(formData),
                        success: function(data2) {
                            alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                            location.reload();
                        }
                    });
                    
                }
                else {
                    alert('The values you entered are not valid. Enter valid values to set an attribute');
                }
                /* location.reload(); */
            }
        })
    });

    $('#addRect').submit(function(e) {
        e.preventDefault();

        var x = $('#x').val();
        var y = $('#y').val();
        var width = $('#width').val();
        var height = $('#height').val();
        var units = $('#rect-units').val();
        var file = $('#add-file').val();

        var formData = '{"x":"' + x + '","y":"' + y + '","width":"' + width + '","height":"' + height + '","units":"' + units + '","file":"' + file + '"}';

        $.ajax({
            type:'POST',
            url:'/addrectangle',
            cache:false,
            processData:false,
            contentType: 'text/plain',
            data:formData,
            success: function(data) {
                if (data.toString() == 1) {
                    let formData;

                    formData = {'username': sessionStorage.getItem('username'),'password': sessionStorage.getItem('password') , 'database': sessionStorage.getItem('database') ,'type': 'added rectangle', 'filename': file, 'summary': 'new rectangle added to ' + file + ' with values x: ' + x + ', y: ' + y + ', width: ' + width + ', height: ' + height + ', units: ' + units}
                    console.log(formData);
                    $.ajax({
                        type: 'POST',
                        url: '/updatechange',
                        cache: false,
                        processData: false,
                        contentType: 'text/plain',
                        data: JSON.stringify(formData),
                        success: function(data2) {
                            alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                            location.reload();
                        }
                    });
                    
                }
                else {
                    alert('An error occurred when creating the rectangle. Check to make sure values are valid svg');
                }
            }
        })
    });

    $('#addCircle').submit(function(e) {
        e.preventDefault();

        var cx = $('#cx').val();
        var cy = $('#cy').val();
        var r = $('#r').val();
        var units = $('#circle-units').val();
        var file = $('#add-file').val();

        var formData = '{"cx":"' + cx + '","cy":"' + cy + '","r":"' + r + '","units":"' + units + '","file":"' + file + '"}';
        console.log(formData);
        
        $.ajax({
            type:'POST',
            url:'/addcircle',
            cache:false,
            processData:false,
            contentType: 'text/plain',
            data:formData,
            success: function(data) {
                if (data.toString() == 1) {

                    let formData;

                    formData = {'username': sessionStorage.getItem('username'),'password': sessionStorage.getItem('password') , 'database': sessionStorage.getItem('database') ,'type': 'added circle', 'filename': file, 'summary': 'new circle added to ' + file + ' with values cx: ' + cx + ', cy: ' + cy + ', r: ' + r + ', units: ' + units}
                    console.log(formData);
                    $.ajax({
                        type: 'POST',
                        url: '/updatechange',
                        cache: false,
                        processData: false,
                        contentType: 'text/plain',
                        data: JSON.stringify(formData),
                        success: function(data2) {
                            alert("Database has " + data2.N1 + " files, " + data2.N2 + " changes, and " + data2.N3 + " downloads");
                            location.reload();
                        }
                    });
                    
                }
                else {
                    alert('An error occured when creating the circle. Check to make sure values are valid svg');
                }
            } 
        })
    });

    /* let selected = $('#view-panel-select').val();
    createTable(selected); */

    $(document).on('change', '#view-panel-select', function() {
        let selected = $('#view-panel-select').val();
        createTable(selected);
        var formData = JSON.stringify($('#infoEdit').serializeArray());
        console.log("FOOOOOOOOOOOOORRRRRRRRRRMMMMMMMMMMM DATA");
        console.log(formData);
    });

    $(document).on('change', '#show-attributes-select', function() {
        let selected = $('#show-attributes-select').val();
        let file = $('#view-panel-select').val();
        createAttributeTable(selected, file);
    });

    $(document).on('change', '#query-select', function() {
        let selected = $('#query-select').val();
        $('#query-table').children('tr').remove();
        $('#get-created-dates').attr('hidden', true);
        $('#get-changed-dates').attr('hidden', true);
        
        
        console.log(selected);
        if (selected == 'showAll') {
            let formData = {'username': sessionStorage.getItem('username'), 'password': sessionStorage.getItem('password'), 'database': sessionStorage.getItem('database')};

            var newRow = $('<tr class="">HELLO</tr>');
            newRow.append('<th>File name</th>');
            newRow.append('<th>File title</th>');
            newRow.append('<th>File description</th>');
            newRow.append('<th># Rectangles</th>');
            newRow.append('<th># Circles</th>');
            newRow.append('<th># Paths</th>');
            newRow.append('<th># Groups</th>');
            newRow.append('<th>Date created</th>');
            newRow.append('<th>File size</th>');
            newRow.appendTo('#query-table');

            $.ajax({
                type: 'POST',
                url: '/getfiletable',
                cache: false,
                processData: false,
                contentType: 'text/plain',
                data: JSON.stringify(formData),
                success: function(data) {
                    console.log(data);

                    data.forEach(e => {
                        var newRow = $('<tr class="">HELLO</tr>');
                        newRow.append('<td>' + e.file_name + '</td>');
                        newRow.append('<td>' + e.file_title + '</td>');
                        newRow.append('<td>' + e.file_description + '</td>');
                        newRow.append('<td>' + e.n_rect + '</td>');
                        newRow.append('<td>' + e.n_circ + '</td>');
                        newRow.append('<td>' + e.n_path + '</td>');
                        newRow.append('<td>' + e.n_group + '</td>');
                        newRow.append('<td>' + e.creation_time + '</td>');
                        newRow.append('<td>' + e.file_size + ' KB</td>');
                        newRow.appendTo('#query-table');
                    });
                }
            })
        }
        if (selected == 'showCreateDates') {
            $('#get-created-dates').removeAttr('hidden');

            var newRow = $('<tr class="">HELLO</tr>');
            newRow.append('<th>File name</th>');
            newRow.append('<th>File title</th>');
            newRow.append('<th>File description</th>');
            newRow.append('<th># Rectangles</th>');
            newRow.append('<th># Circles</th>');
            newRow.append('<th># Paths</th>');
            newRow.append('<th># Groups</th>');
            newRow.append('<th>Date created</th>');
            newRow.append('<th>File size</th>');
            newRow.appendTo('#query-table');
        }
        if (selected == 'showModifiedDates') {
            $('#get-changed-dates').removeAttr('hidden');

            var newRow = $('<tr class="">HELLO</tr>');
            newRow.append('<th>Modification Date</th>');
            newRow.append('<th># Changes</th>');
            newRow.append('<th>File name</th>');
            newRow.append('<th>File size</th>');
            newRow.appendTo('#query-table');
        }
        
    });

    $('#get-changed-dates').submit(function(e) {
        e.preventDefault();

        let formData = {'date1': $('#changed-date-1').val() , 'date2': $('#changed-date-2').val(), 'username': sessionStorage.getItem('username'), 'password': sessionStorage.getItem('password'), 'database': sessionStorage.getItem('database')};
        console.log(formData);

        $.ajax({
            type: 'POST',
            url: '/getchangeddates',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: JSON.stringify(formData),
            success: function(data) {
                console.log(data);

                for (let i = 0; i < data.change.length; i++) {
                    var newRow = $('<tr class="">HELLO</tr>');
                    newRow.append('<td>' + data.change[i].change_time + '</td>');
                    newRow.append('<td>' + 1 + '</td>');
                    newRow.append('<td>' + data.file[i].file_name + '</td>');
                    newRow.append('<td>' + data.file[i].file_size + ' KB</td>');
                    newRow.appendTo('#query-table');
                }
            }
        })
    });

    $('#get-created-dates').submit(function(e) {

        e.preventDefault();

        let formData = {'date1': $('#created-date-1').val() , 'date2': $('#created-date-2').val(), 'username': sessionStorage.getItem('username'), 'password': sessionStorage.getItem('password'), 'database': sessionStorage.getItem('database')};
        console.log(formData);
        $.ajax({
            type: 'POST',
            url: '/getcreateddates',
            cache: false,
            processData: false,
            contentType: 'text/plain',
            data: JSON.stringify(formData),
            success: function(data) {
                console.log(data);
                
                data.forEach(e => {
                    var newRow = $('<tr class="">HELLO</tr>');
                    newRow.append('<td>' + e.file_name + '</td>');
                    newRow.append('<td>' + e.file_title + '</td>');
                    newRow.append('<td>' + e.file_description + '</td>');
                    newRow.append('<td>' + e.n_rect + '</td>');
                    newRow.append('<td>' + e.n_circ + '</td>');
                    newRow.append('<td>' + e.n_path + '</td>');
                    newRow.append('<td>' + e.n_group + '</td>');
                    newRow.append('<td>' + e.creation_time + '</td>');
                    newRow.append('<td>' + e.file_size + '</td>');
                    newRow.appendTo('#query-table');
                });
            }
        }) 
    })
});

