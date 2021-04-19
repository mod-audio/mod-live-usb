/* custom js for live-mod */

$('document').ready(function() {
    // network and controller ping times
    $('#mod-status').show().statusTooltip('updatePosition')

    // xrun counter
    $('#mod-xruns').show()

    // CPU speed and temperature
    $('#mod-cpu-stats').show()

    // transport parameters
    $('#mod-transport-window').css({
        right: '325px'
    })
});
