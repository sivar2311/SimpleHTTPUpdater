const express = require('express')
const app = express()
const port = 80

app.use(express.static('firmware'));

app.listen(port, () => {
    console.log(`Updateserver listening  on port ${port}`)
})