/* USAGE 
copy the json to the input function, then 
comment\uncomment the correct concert code lines 
*/

const input = inputJson();

// convert firestore json to json
const jsonData = documentToJson(input);
console.log(JSON.stringify(jsonData, null, '    '));

// // convert json to firestore json
// const documentData = jsonToDocument(input).mapValue.fields;
// console.log(JSON.stringify(documentData, null, '\t'));


// input function
function inputJson() {
    return {
        "nickname": {
            "stringValue": "tester"
        },
        "firmware": {
            "stringValue": "esp8266_v0.1"
        },
        "mesh_port": {
            "integerValue": "5555"
        },
        "mesh_password": {
            "stringValue": "somethingSneaky"
        },
        "mesh_prefix": {
            "stringValue": "whateverYouLike"
        }
    };
}

/*jsonToDocumenton and documentToJson are based on https://stackoverflow.com/questions/62246410/how-to-convert-a-firestore-document-to-plain-json-and-vice-versa*/
function jsonToDocument(value) {
    if (value == null) {
        return { "nullValue": null };
    } else if (!isNaN(value)) {
        if (value.toString().indexOf(".") != -1) {
            return { "doubleValue": value };
        } else {
            return { "integerValue": value };
        }
    } else if (value === "true" || value === "false" || typeof value == "boolean") {
        return { "booleanValue": value };
    } else if (Date.parse(value)) {
        return { "timestampValue": value };
    } else if (typeof value == "string") {
        return { "stringValue": value };
    } else if (value && value.constructor === Array) {
        return { "arrayValue": { values: value.map((v) => jsonToDocument(v)) } };
    } else if (typeof value === "object") {
        const obj = {};
        for (const o in value) {
            obj[o] = jsonToDocument(value[o]);
        }
        return { "mapValue": { fields: obj } };
    }
}

/* */
function documentToJson(fields) {
    const result = {};
    for (const f in fields) {
        const key = f;
        const value = fields[f];
        const isDocumentType = ["stringValue", "booleanValue", "doubleValue",
            "integerValue", "timestampValue", "mapValue", "arrayValue", "nullValue",
        ].find((t) => t === key);
        if (isDocumentType) {
            const item = ["stringValue", "booleanValue", "doubleValue",
                "integerValue", "timestampValue", "nullValue"]
                .find((t) => t === key);
            if (item) {
                return value;
            } else if ("mapValue" == key) {
                return documentToJson(value.fields || {});
            } else if ("arrayValue" == key) {
                const list = value.values;
                return list ? list.map((l) => documentToJson(l)) : [];
            }
        } else {
            result[key] = documentToJson(value);
        }
    }
    return result;
}
